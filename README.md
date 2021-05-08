# 3DZwei

3DZwei is a memory cardgame clone for Nintendo 3DS.

## Main Features
* 3DZwei includes 3 AI Methods / Difficulties, and those are explained below!

### Method 1: Random
This method does what it says -> playing randomly.

### Method 2: Hard
This method stores the last played cards as a vector of pairs. On the second Turn State, it checks through all the cards in that vector to see, if a card matches the one of the first Turn State. If it does, it plays it -> Otherwise it uses the Random Method.

### Method 3: Extreme
This is a more improved version of the Hard Method. It already does the thing what the Hard Method does on it's first Turn State. NOTE, that it may take longer the more turns have passed, because it does check for the first card pairs, second card pairs and mixed.. if it can find a valid pair already which got played. On the second Turn State, it switches over to the Hard Method, because we don't need more checks than really required.


* A new UI based of [Sim2Editor](https://github.com/Universal-Team/Sim2Editor) ones.

* Ability for custom Cardsets and Charactersets! See [here](https://github.com/Universal-Team/3DZwei/tree/Rewrite/set-generator/README.md) for more. The wiki page [here](https://github.com/Universal-Team/3DZwei/wiki) though will contain a more detailed guide when the next version (v0.3.0) is out.

* A `Least amount of tries` (`Try Play`) and `Normal` (`Normal Play`) Play Mode.

* Multi-Player on a single console, if the AI is disabled.

## Screenshots

<details><summary>Screenshots</summary>

### AI Selector
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/aiSelector.png)

### Card Selector
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/cardSelector.png)

### Cardset Preview
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/cardsetPreview.png)

### Characterset Preview
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/charsetPreview.png)

### Character Selector
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/characterSelector.png)

### Credits
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/creditsContributors.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/creditsStackMemCore.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/creditsUniversalCore.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/creditsTranslators.png)

### Game Result
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/gameResult.png)

### Game Screen
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/gameScreenNormal.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/gameScreenTries.png)

### Game Settings
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/gameSettingsGeneral.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/gameSettingsPlayers.png)

### Keyboard
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/keyboard.png)

### Language Selector
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/languageSelector.png)

### Main Menu
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/mainmenu.png)

### Numpad
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/numpad.png)

### Rules
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/rules.png)

### Set Selector
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/setSelector.png)

### Settings
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/settingsConfig.png) ![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/settingsAppInfo.png)

### Splash
![](https://github.com/Universal-Team/3DZwei/blob/Rewrite/resources/screenshots/splash.png)

</details>

## Credits
### Main Developers
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ)

### Others
- [devkitPro](https://github.com/devkitPro): devkitARM, Libctru, Citro2D & Citro3D.
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): StackMem-Core + 3DZwei developement.
- [Universal-Team](https://github.com/Universal-Team): Universal-Core.

### Translators
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): Deutsch, English.