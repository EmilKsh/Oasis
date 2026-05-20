#include "RealTimePlot.h"

#include "RenderSystem.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>

namespace render_system
{
	namespace
	{
		std::string FormatValue(double value)
		{
			std::ostringstream out;
			const double absValue = std::abs(value);
			if (absValue > 999.0 || (absValue > 0.0 && absValue < 0.01))
				out << std::scientific << std::setprecision(1) << value;
			else
				out << std::fixed << std::setprecision(2) << value;
			return out.str();
		}

		double NicePadding(double minY, double maxY)
		{
			const double span = maxY - minY;
			if (!(span > 0.0))
				return std::max(1.0, std::abs(maxY) * 0.1);
			return span * 0.08;
		}
	}

	RealTimePlot::RealTimePlot(std::string title) : title{ std::move(title) }
	{
	}

	size_t RealTimePlot::AddSeries(const std::string& name, glm::vec3 color)
	{
		for (size_t i = 0; i < seriesList.size(); ++i)
		{
			if (seriesList[i].name == name)
				return i;
		}

		seriesList.push_back(Series{ name, color, {} });
		return seriesList.size() - 1;
	}

	void RealTimePlot::PushSample(size_t seriesIndex, double x, double y)
	{
		if (seriesIndex >= seriesList.size())
			return;

		Series& series = seriesList[seriesIndex];
		series.samples.push_back(Sample{ x, y });
		TrimSeries(series, x);
	}

	void RealTimePlot::PushSample(const std::string& seriesName, double x, double y)
	{
		PushSample(FindOrAddSeries(seriesName), x, y);
	}

	void RealTimePlot::Clear()
	{
		for (Series& series : seriesList)
			series.samples.clear();
	}

	void RealTimePlot::SetViewport(float x, float y, float width, float height)
	{
		viewport = glm::vec4(x, y, width, height);
	}

	void RealTimePlot::SetYRange(double minY, double maxY)
	{
		autoScale = false;
		fixedMinY = minY;
		fixedMaxY = maxY;
	}

	void RealTimePlot::Render(RenderSystem& renderer)
	{
		const glm::vec3 panelColor{ 0.055f, 0.062f, 0.075f };
		const glm::vec3 innerColor{ 0.075f, 0.087f, 0.105f };
		const glm::vec3 borderColor{ 0.38f, 0.43f, 0.50f };
		const glm::vec3 textColor{ 0.88f, 0.91f, 0.94f };

		renderer.DrawFilledRect(viewport.x, viewport.y, viewport.z, viewport.w, panelColor);

		const float inset = 0.045f;
		const glm::vec4 plotArea{
			viewport.x + inset,
			viewport.y + inset,
			viewport.z - inset * 1.45f,
			viewport.w - inset * 1.85f
		};
		const glm::vec4 oldViewport = viewport;
		viewport = plotArea;

		renderer.DrawFilledRect(plotArea.x, plotArea.y, plotArea.z, plotArea.w, innerColor);

		double minX{}, maxX{}, minY{}, maxY{};
		const bool hasData = ComputeRanges(minX, maxX, minY, maxY);
		if (hasData)
		{
			DrawGrid(renderer, minY, maxY);

			for (const Series& series : seriesList)
			{
				if (series.samples.size() < 2)
					continue;

				for (size_t i = 1; i < series.samples.size(); ++i)
				{
					const Sample& a = series.samples[i - 1];
					const Sample& b = series.samples[i];
					if (a.x < minX || b.x < minX)
						continue;

					const glm::vec2 p0 = PlotToScreen(a, minX, maxX, minY, maxY);
					const glm::vec2 p1 = PlotToScreen(b, minX, maxX, minY, maxY);
					renderer.DrawLine(p0.x, p0.y, p1.x, p1.y, series.color, 2.0f);
				}

				const Sample& latest = series.samples.back();
				if (latest.x >= minX)
				{
					const glm::vec2 p = PlotToScreen(latest, minX, maxX, minY, maxY);
					renderer.DrawFilledRect(p.x - 0.006f, p.y - 0.006f, 0.012f, 0.012f, series.color);
				}
			}
		}

		renderer.DrawLine(plotArea.x, plotArea.y, plotArea.x + plotArea.z, plotArea.y, borderColor, 1.5f);
		renderer.DrawLine(plotArea.x, plotArea.y + plotArea.w, plotArea.x + plotArea.z, plotArea.y + plotArea.w, borderColor, 1.0f);
		renderer.DrawLine(plotArea.x, plotArea.y, plotArea.x, plotArea.y + plotArea.w, borderColor, 1.0f);
		renderer.DrawLine(plotArea.x + plotArea.z, plotArea.y, plotArea.x + plotArea.z, plotArea.y + plotArea.w, borderColor, 1.0f);

		viewport = oldViewport;
		DrawText(renderer, title, glm::vec2(viewport.x + 0.025f, viewport.y + viewport.w - 0.048f), 0.30f, textColor);

		if (hasData)
		{
			DrawText(renderer, FormatValue(maxY), glm::vec2(plotArea.x + 0.006f, plotArea.y + plotArea.w - 0.025f), 0.22f, glm::vec3(0.66f, 0.71f, 0.78f));
			DrawText(renderer, FormatValue(minY), glm::vec2(plotArea.x + 0.006f, plotArea.y + 0.008f), 0.22f, glm::vec3(0.66f, 0.71f, 0.78f));
		}

		float legendX = viewport.x + viewport.z - 0.20f;
		float legendY = viewport.y + viewport.w - 0.055f;
		for (const Series& series : seriesList)
		{
			renderer.DrawFilledRect(legendX, legendY + 0.003f, 0.020f, 0.010f, series.color);
			DrawText(renderer, series.name, glm::vec2(legendX + 0.028f, legendY - 0.004f), 0.22f, textColor);
			legendY -= 0.036f;
		}
	}

	size_t RealTimePlot::FindOrAddSeries(const std::string& name)
	{
		static const glm::vec3 palette[] = {
			{ 0.25f, 0.78f, 1.00f },
			{ 1.00f, 0.66f, 0.25f },
			{ 0.55f, 0.92f, 0.47f },
			{ 0.95f, 0.42f, 0.70f },
			{ 0.74f, 0.62f, 1.00f }
		};
		return AddSeries(name, palette[seriesList.size() % std::size(palette)]);
	}

	void RealTimePlot::TrimSeries(Series& series, double newestX)
	{
		while (series.samples.size() > maxSamples)
			series.samples.pop_front();

		if (timeWindow > 0.0)
		{
			const double minX = newestX - timeWindow;
			while (!series.samples.empty() && series.samples.front().x < minX)
				series.samples.pop_front();
		}
	}

	bool RealTimePlot::ComputeRanges(double& minX, double& maxX, double& minY, double& maxY) const
	{
		minX = std::numeric_limits<double>::max();
		maxX = std::numeric_limits<double>::lowest();
		minY = std::numeric_limits<double>::max();
		maxY = std::numeric_limits<double>::lowest();

		for (const Series& series : seriesList)
		{
			for (const Sample& sample : series.samples)
			{
				minX = std::min(minX, sample.x);
				maxX = std::max(maxX, sample.x);
				minY = std::min(minY, sample.y);
				maxY = std::max(maxY, sample.y);
			}
		}

		if (minX == std::numeric_limits<double>::max())
			return false;

		if (timeWindow > 0.0)
			minX = std::max(minX, maxX - timeWindow);

		if (maxX <= minX)
			maxX = minX + 1.0;

		if (!autoScale)
		{
			minY = fixedMinY;
			maxY = fixedMaxY;
		}

		const double padding = NicePadding(minY, maxY);
		minY -= padding;
		maxY += padding;

		if (maxY <= minY)
			maxY = minY + 1.0;

		return true;
	}

	glm::vec2 RealTimePlot::PlotToScreen(const Sample& sample, double minX, double maxX, double minY, double maxY) const
	{
		const double tx = (sample.x - minX) / (maxX - minX);
		const double ty = (sample.y - minY) / (maxY - minY);
		return glm::vec2(
			viewport.x + viewport.z * static_cast<float>(std::clamp(tx, 0.0, 1.0)),
			viewport.y + viewport.w * static_cast<float>(std::clamp(ty, 0.0, 1.0))
		);
	}

	glm::vec2 RealTimePlot::NdcToPixels(const RenderSystem& renderer, glm::vec2 ndc) const
	{
		return glm::vec2(
			(ndc.x * 0.5f + 0.5f) * renderer.WindowSize[0],
			(ndc.y * 0.5f + 0.5f) * renderer.WindowSize[1]
		);
	}

	void RealTimePlot::DrawGrid(RenderSystem& renderer, double minY, double maxY)
	{
		const glm::vec3 minorGrid{ 0.15f, 0.17f, 0.20f };
		const glm::vec3 majorGrid{ 0.22f, 0.25f, 0.30f };
		const int verticalLines = 8;
		const int horizontalLines = 5;

		for (int i = 0; i <= verticalLines; ++i)
		{
			const float t = static_cast<float>(i) / verticalLines;
			const float x = viewport.x + viewport.z * t;
			renderer.DrawLine(x, viewport.y, x, viewport.y + viewport.w, i == verticalLines ? majorGrid : minorGrid, 1.0f);
		}

		for (int i = 0; i <= horizontalLines; ++i)
		{
			const float t = static_cast<float>(i) / horizontalLines;
			const float y = viewport.y + viewport.w * t;
			renderer.DrawLine(viewport.x, y, viewport.x + viewport.z, y, i == 0 ? majorGrid : minorGrid, 1.0f);
		}

		if (minY < 0.0 && maxY > 0.0)
		{
			const float y0 = viewport.y + viewport.w * static_cast<float>((0.0 - minY) / (maxY - minY));
			renderer.DrawLine(viewport.x, y0, viewport.x + viewport.z, y0, glm::vec3(0.38f, 0.43f, 0.50f), 1.5f);
		}
	}

	void RealTimePlot::DrawText(RenderSystem& renderer, const std::string& text, glm::vec2 ndc, float scale, glm::vec3 color) const
	{
		const glm::vec2 pixels = NdcToPixels(renderer, ndc);
		renderer.RenderText(text, pixels.x, pixels.y, scale, color);
	}
}
