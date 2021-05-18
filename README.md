# 3DZwei
<p align="center">
	<a href="https://github.com/Universal-Team/3DZwei/releases/latest"><img src="https://github.com/Universal-Team/3DZwei/blob/master/resources/logos/logo.png"></a><br>
	<b>A memory cardgame clone for Nintendo 3DS.</b><br>
</p>

## Main Features
* 3DZwei includes 3 AI Methods / Difficulties, and those are explained below!

### Method 1: Random
This method does what it says -> playing randomly.

### Method 2: Hard
This method stores the last played cards as a vector of pairs. On the second Turn State, it checks through all the cards in that vector to see, if a card matches the one of the first Turn State. If it does, it plays it -> Otherwise it uses the Random Method.

### Method 3: Extreme
This is a more improved version of the Hard Method. It already does the thing what the Hard Method does on it's first Turn State. NOTE, that it may take longer the more turns have passed, because it does check for the first card pairs, second card pairs and mixed.. if it can find a valid pair already which got played. On the second Turn State, it switches over to the Hard Method, because we don't need more checks than really required.


* A new UI based of [Sim2Editor](https://github.com/Universal-Team/Sim2Editor) ones.

* Ability for custom Cardsets and Charactersets! See [here](https://github.com/Universal-Team/3DZwei/tree/master/set-generator/README.md) for more. The wiki page [here](https://github.com/Universal-Team/3DZwei/wiki) though will contain a more detailed guide when the next version (v0.3.0) is out.

* Versus and Solo Play mode.

* Multi-Player on a single console, if the AI is disabled.

## Screenshots

<details><summary>Screenshots</summary>

### AI Selector
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/aiSelector.png)

### Card Selector
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/cardSelector.png)

### Character Selector
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/characterSelector.png)

### Credits
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/creditsContributors.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/creditsStackMemCore.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/creditsUniversalCore.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/creditsTranslators.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/creditsSpecialThanks.png)

### Game Result
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/gameResult.png)

### Game Screen
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/gameScreenVersus.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/gameScreenSolo.png)

### Game Settings
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/gameSettingsGeneral.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/gameSettingsPlayers.png)

### Language Selector
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/languageSelector.png)

### Main Menu
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/mainmenu.png)

### Rules
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/rules.png)

### Set Selectors
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/cardsetSelectorList.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/cardsetSelectorCard.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/charactersetSelectorList.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/charactersetSelectorChar.png)

### Settings
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/settingsConfig.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/settingsAnimation.png) ![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/settingsAppInfo.png)

### Splash
![](https://github.com/Universal-Team/3DZwei/blob/master/resources/screenshots/splash.png)

</details>

## Compilation

### Setting up your environment

To build 3DZwei from source, you will need to install devkitARM, libctru, citro2d and citro3d. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run the following command to install everything you need:
```
(sudo dkp-)pacman -S 3ds-dev
```
(The `sudo dkp-` may not be needed depending on your OS).

You will also need [bannertool](https://github.com/Steveice10/bannertool/releases/latest) and [makerom](https://github.com/profi200/Project_CTR/releases/latest) in your PATH.

### Cloning the repository

To download the source you will need to clone the repository with submodules, this can be done by running:
```
git clone --recursive https://github.com/Universal-Team/3DZwei.git
```

If you've already cloned it, you can use the following command to update all of the submodules:
```
git submodule update --init --recursive
```

### Building

Once you've cloned the repository (with submodules), simply run `make` in the root of the repository. You will find `3DZwei.cia` and `3DZwei.3dsx` inside the `3ds` directory.


## Getting Sets
You can find user-created Sets on the [Universal-Team Game Sets](https://game-sets.universal-team.net/) page

This also exists in the form of a UniStore for [Universal-Updater](https://github.com/Universal-Team/Universal-Updater). You just need to find it in the list of the recommended UniStores. If you don't know how, see below for instructions.

1.) Press on the last Tab on the left side in Universal-Updater.

2.) Press on `Select UniStore`.

3.) Press on the small `+` circle icon on the bottom screen.

4.) Find the `Universal-Team Game Sets` entry in the list and select it.

5.) Now press on `ut-game-sets.unistore` and you are ready to go!


## Credits
### Main Developers
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ)

### Others
- [devkitPro](https://github.com/devkitPro): devkitARM, Libctru, Citro2D & Citro3D.
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): StackMem-Core + 3DZwei developement.
- [Universal-Team](https://github.com/Universal-Team): Universal-Core.

### Special Thanks
- [NightScript](https://github.com/NightYoshi370): For having the idea to use cubic bezier for animations, the falling cards animation on the game start and suggestions.
- [Pk11](https://github.com/Epicpkmn11): For helping me with an example on how to flip a card in 2D and suggestions.
- Universal-Microwave: For better teaching me how cubic bezier works with an example and suggestions.

### Translators
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): Deutsch, English.
