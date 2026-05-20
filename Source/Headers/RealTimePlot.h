#ifndef REAL_TIME_PLOT_H
#define REAL_TIME_PLOT_H

#include <deque>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace render_system
{
	class RenderSystem;

	class RealTimePlot
	{
	public:
		struct Sample
		{
			double x{};
			double y{};
		};

		struct Series
		{
			std::string name;
			glm::vec3 color{};
			std::deque<Sample> samples;
		};

		RealTimePlot(std::string title = "Live diagnostics");

		size_t AddSeries(const std::string& name, glm::vec3 color);
		void PushSample(size_t seriesIndex, double x, double y);
		void PushSample(const std::string& seriesName, double x, double y);
		void Clear();

		void SetViewport(float x, float y, float width, float height);
		void SetMaxSamples(size_t count) { maxSamples = count; }
		void SetTimeWindow(double seconds) { timeWindow = seconds; }
		void SetAutoScale(bool enabled) { autoScale = enabled; }
		void SetYRange(double minY, double maxY);
		void SetTitle(const std::string& value) { title = value; }

		void Render(RenderSystem& renderer);

	private:
		size_t FindOrAddSeries(const std::string& name);
		void TrimSeries(Series& series, double newestX);
		bool ComputeRanges(double& minX, double& maxX, double& minY, double& maxY) const;
		glm::vec2 PlotToScreen(const Sample& sample, double minX, double maxX, double minY, double maxY) const;
		glm::vec2 NdcToPixels(const RenderSystem& renderer, glm::vec2 ndc) const;
		void DrawGrid(RenderSystem& renderer, double minY, double maxY);
		void DrawText(RenderSystem& renderer, const std::string& text, glm::vec2 ndc, float scale, glm::vec3 color) const;

		std::string title;
		std::vector<Series> seriesList;
		glm::vec4 viewport{ -0.95f, -0.92f, 0.82f, 0.48f };
		size_t maxSamples{ 600 };
		double timeWindow{ 8.0 };
		bool autoScale{ true };
		double fixedMinY{ -1.0 };
		double fixedMaxY{ 1.0 };
	};
}

#endif
