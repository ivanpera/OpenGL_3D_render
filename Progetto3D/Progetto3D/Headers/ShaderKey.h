#ifndef SHAD_KEY
#define SHAD_KEY
#include <string>
#include "HashCombine.h"

typedef struct ShaderKey {
	std::string vertShader, fragShader;
	ShaderKey(std::string const vertexShader, std::string const fragmentShader) : vertShader(vertexShader), fragShader(fragmentShader) {}
	bool operator ==(const ShaderKey& b) const {
		return this->fragShader == b.fragShader && this->vertShader == b.vertShader;
	}
	bool operator !=(const ShaderKey& b) const {
		return !(*this == b);
	}
} ShaderKey;

namespace std {

	template<> struct less<ShaderKey> {
		bool operator () (ShaderKey const& lhs, ShaderKey const& rhs) const {
			if (lhs.vertShader < rhs.vertShader) {
				return true;
			}
			return lhs.fragShader < (rhs.fragShader);
		}
	};

	template<> struct hash<ShaderKey> {
		size_t operator()(ShaderKey const& key) const {
			std::size_t seed = 0;
			hash_combine(seed, key.vertShader, key.fragShader);
			return seed;
		}
	};
}

#endif 


