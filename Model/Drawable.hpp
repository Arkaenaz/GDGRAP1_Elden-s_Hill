#ifndef MODELS_DRAWABLE_HPP
#define MODELS_DRAWABLE_HPP

#include <vector>

#include "Shaders.hpp"

namespace models {
	class Drawable {
		public:
			virtual void draw(Shaders& CShaders) = 0;
	};
}

#endif