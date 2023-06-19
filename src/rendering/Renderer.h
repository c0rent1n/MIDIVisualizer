#ifndef Renderer_h
#define Renderer_h
#include <GLFW/glfw3.h>
#include <gl3w/gl3w.h>
#include <glm/glm.hpp>
#include <memory>
#include <array>

#include "Framebuffer.h"
#include "camera/Camera.h"
#include "scene/MIDIScene.h"
#include "ScreenQuad.h"
#include "Score.h"

#include "../helpers/Recorder.h"

#include "State.h"

#include "../libs/miniaudio.h"

#define DEBUG_SPEED (1.0f)

struct SystemAction {
	enum Type {
		NONE, FIX_SIZE, FREE_SIZE, FULLSCREEN, QUIT, RESIZE
	};

	Type type;
	glm::ivec4 data;

	SystemAction(Type action);
};


class Renderer {

public:

	Renderer(const Configuration& config);

	~Renderer();
	
	bool loadMidiFile(const std::string & midiFilePath);
	
	bool loadAudioFile(const std::string & audioFilePath);

	bool connectDevice(const std::string & deviceName);

	void setState(const State & state);
	
	/// Draw function
	SystemAction draw(const float currentTime);
	
	/// Clean function
	void clean();

	/// Handle screen resizing
	void resize(int width, int height);

	/// Handle window content density.
	void rescale(float scale);

	void resizeAndRescale(int width, int height, float scale);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);

	/// Directly start recording.
	bool startDirectRecording(const Export& exporting, const glm::vec2 & size);

	void setGUIScale(float scale);

	void updateConfiguration(Configuration& config);

private:
	

	struct Layer {
		
		enum Type : unsigned int {
			BGCOLOR = 0, BGTEXTURE, BLUR, ANNOTATIONS, KEYBOARD, PARTICLES, NOTES, FLASHES, PEDAL, WAVE, COUNT
		};

		Type type = BGCOLOR;
		std::string name = "None";
		void (Renderer::*draw)(const glm::vec2 &) = nullptr;
		bool * toggle = nullptr;

	};

	void blurPrepass();

	void drawBackgroundImage(const glm::vec2 & invSize);

	void drawBlur(const glm::vec2 & invSize);

	void drawParticles(const glm::vec2 & invSize);

	void drawScore(const glm::vec2 & invSize);

	void drawKeyboard(const glm::vec2 & invSize);

	void drawNotes(const glm::vec2 & invSize);

	void drawFlashes(const glm::vec2 & invSize);

	void drawPedals(const glm::vec2 & invSize);
	
	void drawWaves(const glm::vec2 & invSize);

	SystemAction drawGUI(const float currentTime);

	void drawScene(bool transparentBG);

	SystemAction showTopButtons(double currentTime);

	void showParticleOptions();

	void showKeyboardOptions();

	void showPedalOptions();
	
	void showWaveOptions();

	void showBlurOptions();

	void showScoreOptions();

	void showBackgroundOptions();

	void showBottomButtons();

	void showLayers();

	void showDevices();

	void showSets();

	void showSetEditor();

	void applyBackgroundColor();

	void applyAllSettings();
	
	void updateAudioPosition();
	
	void playPause(float timerStart);
	
	void reset();

	void startRecording();

	void updateSizes();

	bool channelColorEdit(const char * name, const char * displayName, ColorArray & colors);
	
	void updateMinMaxKeys();

	void synchronizeColors(const ColorArray & colors);

	void ImGuiPushItemWidth(int w);

	void ImGuiSameLine(int w = 0);

	State _state;
	std::array<Layer, Layer::COUNT> _layers;
	SetOptions _backupSetOptions;

	float _timer = 0.0f;
	float _timerStart = 0.0f;
	bool _shouldPlay = false;
	bool _showGUI = true;
	bool _showDebug = false;
	bool _verbose = false;

	Recorder _recorder;
	
	Camera _camera;
	
	std::shared_ptr<Framebuffer> _particlesFramebuffer;
	std::shared_ptr<Framebuffer> _blurFramebuffer0;
	std::shared_ptr<Framebuffer> _blurFramebuffer1;
	std::shared_ptr<Framebuffer> _renderFramebuffer;
	std::shared_ptr<Framebuffer> _finalFramebuffer;

	std::shared_ptr<MIDIScene> _scene;
	ScreenQuad _blurringScreen;
	ScreenQuad _passthrough;
	ScreenQuad _backgroundTexture;
	ScreenQuad _fxaa;
	std::shared_ptr<Score> _score;
	
	ma_sound _sound;
	ma_engine _engine;
	std::string _lastAudioPath;
	bool _soundLoaded = false;

	glm::ivec2 _windowSize;
	glm::ivec2 _backbufferSize;
	float _guiScale = 1.0f;
	unsigned int _shouldQuit = 0;
	int _selectedPort = 0;
	bool _showLayers = false;
	bool _showSetListEditor = false;
	bool _exitAfterRecording = false;
	bool _fullscreen = false;
	bool _liveplay = false;
	bool _useTransparency = false;
	const bool _supportTransparency;
};

#endif
