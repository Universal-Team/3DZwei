# 3DZwei Set Generator

With this you can generate your own Card and Character Sets for 3DZwei.

- [x] The maximum cards you can store into a Spritesheet is 323 front covers and 1 back cover.
- [x] The maximum character you can store into a Spritesheet is 64.

**The cards must be 55x55 and the Characters must be 120x120!**

## Building a Set
To build the Sets, you will need to install devkitARM and Tex3ds. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run the following command to install everything you need:
```
(sudo dkp-)pacman -S 3ds-dev
```
(The `sudo dkp-` may not be needed depending on your OS).

After that is done, edit the PNG graphics inside `gfx/cards/` and or `gfx/chars/`. Do NOTE, that in case you added a new card or character, you'll have to edit the `t3s` files like this:

```
--atlas -f rgba8888 -z auto

cards/card_0.png
cards/card_1.png
cards/card_2.png
cards/card_3.png
cards/card_4.png
cards/card_5.png
cards/card_6.png
cards/card_7.png
cards/card_8.png
cards/card_9.png

cards/NewCardName.png

cards/card_empty.png
```

NOTE: For the card set, the last graphic listed is the back cover. So add all your actually wanted cards BEFORE that. For the character set, you can ignore this.

If you are done with adding your graphics, simply run `make` in the `set-generator` directory. You will find your sets inside the `out` directory.

Rename the `t3x` files to whatever you like, and place them in the following directories on your SD Card:

`cards.t3x`: `sdmc:/3ds/ut-games/sets/3DZwei/`

`chars.t3x`: `sdmc:/3ds/ut-games/sets/characters/`


## Loading the Sets

1.) Go into the Settings, then find `Active Cardset` or `Active Characterset`.

2.) Click on the 3 stripes next to the option. You should now be able to find your placed t3x file through a file selector.

3.) After you selected a Set, it will show a preview of it. At this point, you can select if you want to use it, or not through the touch screen buttons, or you can press `B` / `Y` to cancel, or `START` / `X` to confirm.

Now have fun with your sets!