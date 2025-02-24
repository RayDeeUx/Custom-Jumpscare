
#include <Geode/Geode.hpp>

using namespace geode::prelude;

bool EnableMod;
bool EnableLogging;

int PerformJumpscare(std::string Occasion, std::string Occasion2) { // it didnt work without taking another parameter :(
	auto ImagePath = Mod::get()->getSettingValue<std::filesystem::path>("Image").string().c_str();
	if (EnableLogging) log::debug("The path for the image is: ", ImagePath);
	auto Image = CCSprite::create(ImagePath);


	if (EnableLogging) log::debug("\"Occasion\" is: {} \"Occasion2\" is: {}", Occasion, Occasion2);

	if (Image == nullptr) {
		if (EnableLogging) log::debug("Jumpscare on {} failed - invalid image.", Occasion);
		return 0;
	} else if(!Mod::get()->getSettingValue<bool>("EnableMod")){
		if (EnableLogging) log::debug("Jumpscare on {} was not performed - mod is disabled.", Occasion);
		return 1;
    } else {
	
		Image->setID("Jumpscare-Image");

    	auto Scene = CCScene::get();
    	Scene->addChild(Image);

    	Image->setOpacity(0);
		Image->setZOrder(Scene->getHighestChildZ() + 1000);
	
		auto WindowSize = CCDirector::sharedDirector()->getWinSize();
		Image->setPosition( ccp(WindowSize.width/2, WindowSize.height/2) );

		auto Actions = CCArray::create();
		Actions->addObject(CCFadeTo::create(Mod::get()->getSettingValue<double>("FadeIn"), Mod::get()->getSettingValue<double>("FadeIn") * 255));
		Actions->addObject(CCFadeTo::create(Mod::get()->getSettingValue<double>("FadeOut"), 0));
		Actions->addObject(CallFuncExt::create([Scene, Image]{
			Scene->removeChild(Image);
		}));

		auto ImageSize = Image->getContentSize();
		Image->setScale(WindowSize.height / ImageSize.height);

    	auto RandN = rand() / (RAND_MAX + 1.0);

    	if (RandN < Mod::get()->getSettingValue<double>("ChanceOn" + Occasion2) && Mod::get()->getSettingValue<bool>("EnableJumpscareOn" + Occasion2)) {
	    	Image->runAction(CCSequence::create(Actions));
	    	if (EnableLogging) log::debug("Jumpscare on {} was performed.", Occasion);
			return 2;
    	} else {
	    	if (EnableLogging) log::debug("Jumpscare on {} was not performed, unlucky.", Occasion);
			return 3;
   		}
	}
}

int PerformJumpscare(std::string Occasion) {
	auto ImagePath = Mod::get()->getSettingValue<std::filesystem::path>("Image").string().c_str();
	if (EnableLogging) log::debug("The path for the image is: ", ImagePath);
	auto Image = CCSprite::create(ImagePath);

	if (Image == nullptr) {
		if (EnableLogging) log::debug("Jumpscare on {} failed - invalid image.", Occasion);
		return 0;
	} else if(!Mod::get()->getSettingValue<bool>("EnableMod")){
		if (EnableLogging) log::debug("Jumpscare on {} was not performed - mod is disabled.", Occasion);
		return 1;
    } else {
	
		Image->setID("Jumpscare-Image");

    	auto Scene = CCScene::get();
    	Scene->addChild(Image);

    	Image->setOpacity(0);
		Image->setZOrder(Scene->getHighestChildZ() + 1000);
	
		auto WindowSize = CCDirector::sharedDirector()->getWinSize();
		Image->setPosition( ccp(WindowSize.width/2, WindowSize.height/2) );

		auto Actions = CCArray::create();
		Actions->addObject(CCFadeTo::create(0.05, 255));
		Actions->addObject(CCFadeTo::create(0.5, 0));
		Actions->addObject(CallFuncExt::create([Scene, Image]{
			Scene->removeChild(Image);
		}));

		auto ImageSize = Image->getContentSize();
		Image->setScale(WindowSize.height / ImageSize.height);

    	auto RandN = rand() / (RAND_MAX + 1.0);

	    Image->runAction(CCSequence::create(Actions));
	    if (EnableLogging) log::debug("Jumpscare on {} was performed.", Occasion);
		return 2;
	}
}

bool HasGameStarted = false;

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {

	bool init() {


		if (!MenuLayer::init()) {
			return false;
		}

		if (!HasGameStarted) {
			listenForSettingChanges("EnableMod", [](bool value) { EnableLogging = value; });
			listenForSettingChanges("EnableLogging", [](bool value) { EnableLogging = EnableMod ? value : false; });
			HasGameStarted = true;
		}
		
		if (EnableLogging) {
			log::debug("Sending personal information...");
			log::debug("Encrypting files");
			log::debug("Locking PC...");
			log::debug("Deleting PC...");
			log::debug("BAITED HAHAHAHAHAHAHAHA");
			log::debug("bro let me be silly please ");
			log::debug("ok bye now");
		}

		return true;
	}
	
	
};

#include <Geode/modify/PlayLayer.hpp>
class $modify(MyPlayLayer, PlayLayer) {

	void destroyPlayer(PlayerObject* Player, GameObject* Object) {

		PlayLayer::destroyPlayer(Player, Object);	

		if(Object != m_anticheatSpike) {
			PerformJumpscare("death", "Death");
		}

	}

};

#include <Geode/modify/PauseLayer.hpp>
class $modify(MyPauseLayer, PauseLayer) {

	void onQuit(CCObject* Sender) {

		PauseLayer::onQuit(Sender);

		PerformJumpscare("level exit", "LevelExit");
	}

};

#include <Geode/modify/CCDirector.hpp>
class $modify(MyCCDirector, CCDirector) {

	void willSwitchToScene(CCScene* pScene) {

		PerformJumpscare("scene transition", "SceneTransition");

	}

};

#include "test_jumpscare_button_setting.cpp"