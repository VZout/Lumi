#pragma once

#include <optional>

namespace lum
{

	class GraphicsSystem
	{
	public:
		void Init(/* WindowDetails, flags etc*/) {}
		void NewFrame() {}
		std::optional<void*> Render(/* Scene graph, Render Tasks, flags */) { return {}; } // returns optional texture in case the render tasks don't render to the back buffer.
		void Present() {}

	private:
		// All render functions and other stuff nessessary to render a scene graph.
	
	};

} /* lum */
