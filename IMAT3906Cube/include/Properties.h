#pragma once
#include <iostream>
#include <vector>

struct IntProp {
	std::string name;
	int value;
	int max;
	std::vector<std::string> outputs;
};

struct BoolProp {
	std::string name;
	bool value;
};

class Properties {
private:
	BoolProp DL;
	BoolProp PL;
	BoolProp SL;
	BoolProp torch;
	BoolProp normalMap;
	BoolProp depth;
	BoolProp hdr;

	IntProp colShader;
	IntProp multiShader;
	IntProp px;
	IntProp blurBloom;

public:

	void populate() {
		DL.name = "DL";
		DL.value = true;
		PL.name = "PL";
		PL.value = true;
		SL.name = "SL";
		SL.value = true;
		torch.name = "torch";
		torch.value = true;
		normalMap.name = "normalMap";
		normalMap.value = true;
		depth.name = "depth";
		depth.value = false;
		hdr.name = "hdr";
		hdr.value = false;

		colShader.name = "colShader";
		colShader.value = 0;
		colShader.max = 3;
		colShader.outputs = std::vector<std::string>{
			"Colour PP shader disabled",
			"Colour PP shader set to greyscale",
			"Colour PP shader set to inverted",
			"Colour PP shader set to night vision"
		};


		multiShader.name = "";
		multiShader.value = 0;
		multiShader.max = 1;
		multiShader.outputs = std::vector<std::string>{
			"Multi Shader disabled",
			"Multi Shader set to brightness filtering"
		};

		px.name = "px";
		px.value = 1;
		px.max = 3;
		px.outputs = std::vector<std::string>{
			"Parallax disabled",
			"Parallax set to standard",
			"Parallax set to steep",
			"Parallax set to occlusion"
		};

		blurBloom.name = "blurBloom";
		blurBloom.value = 0;
		blurBloom.max = 2;
		blurBloom.outputs = std::vector<std::string>{
			"Blur & Bloom disabled",
			"Blur enabled",
			"Bloom enabled"
		};
	}

	inline BoolProp getDL() { return DL; }
	inline BoolProp getPL() { return PL; }
	inline BoolProp getSL() { return SL; }
	inline BoolProp getTorch() { return torch; }
	inline BoolProp getNormalMap() { return normalMap; }
	inline BoolProp getDepth() { return depth; }
	inline BoolProp getHDR() { return hdr; }

	inline IntProp getPx() { return px; }
	inline IntProp getColShader() { return colShader; }
	inline IntProp getMultiShader() { return multiShader; }
	inline IntProp getBlurBloom() { return blurBloom; }

	inline void setDL(bool b) { DL.value = b; }
	inline void setPL(bool b) { PL.value = b; }
	inline void setSL(bool b) { SL.value = b; }
	inline void setTorch(bool b) { torch.value = b; }
	inline void setNormalMap(bool b) { normalMap.value = b; }
	inline void setDepth(bool b) { depth.value = b; }
	inline void setHDR(bool b) { hdr.value = b; }

	inline void setPx(int i) { px.value = i; }
	inline void setPx(IntProp i) { px = i; }
	inline void setColShader(int i) { colShader.value = i; }
	inline void setColShader(IntProp i) { colShader = i; }
	inline void setMultiShader(int i) { multiShader.value = i; }
	inline void setMultiShader(IntProp i) { multiShader = i; }
	inline void setBlurBloom(int i) { blurBloom.value = i; }
	inline void setBlurBloom(IntProp i) { blurBloom = i; }

	IntProp cycleIntProp(IntProp prop) {
		if (prop.value < prop.max) {
			prop.value++;
		}
		else {
			prop.value = 0;
		}
		return prop;
	}

};