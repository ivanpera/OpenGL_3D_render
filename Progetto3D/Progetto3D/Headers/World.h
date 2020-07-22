#pragma once
#include <vector>
#include <memory>
#include <map>
#include "common.h"
#include "GameObject.h"
#include "Light.h"
#include "Camera.h"

enum class GameState {
	Pause, Playing, Over
};

class World {
private:
	unsigned int worldW, worldH;
	GameState gameState = GameState::Playing;
	std::vector<std::shared_ptr<Camera>> cameras;
	std::vector<std::shared_ptr<Light>> lights;
	std::vector<std::shared_ptr<GameObject>> objects;
	std::map<std::string, std::shared_ptr<GameObject>> assets;
public:
	World(unsigned int const width, unsigned int const height);
	void onKeyDown(std::string key, int time, float deltaTime);
	void onKeyRelease(std::string key, int time, float deltaTime);
	void update(int const time, float const deltaTime);
	void loadAssets();
	void loadScene();
	std::vector<std::shared_ptr<GameObject>> const& getObjects() const;
	std::vector<std::shared_ptr<Light>> const& getLights() const;
	std::vector<std::shared_ptr<Camera>> const& getCameras() const;
	std::vector<std::shared_ptr<GameObject>> const getAssets() const;
	std::shared_ptr<GameObject> const& getAsset(std::string name) const;

	template<class T>
	std::shared_ptr<T>& addObject(T const& obj) {
		objects.push_back(std::shared_ptr<T>(new T(obj)));
		return objects.back();
	};

	template<class T>
	void addAsset(T const& obj, std::string name) {
		assets.insert({ name, std::shared_ptr<T>(new T(obj)) });
	};

	template<class T>
	std::shared_ptr<T>& addObject(std::shared_ptr<T> const& pObj) {
		objects.push_back(pObj);
		return objects.back();
	};

	std::shared_ptr<Camera>& addCamera(Camera const& obj);

	std::shared_ptr<Light>& addLight(Light const& obj);
};
