# foodsavr
~30% of food in the world is wasted and sent to landfills. foodsavr is a gadget that magnetically attaches to your fridge to remind you what food is close to expiring.

## Usage
To add a food item:
Show your food item to the camera and press the A button. Then, enter the expiry date of the item with the number pad. Then, confirm by pressing the A button again. Press D to cancel.

Press B and C to cycle up and down through the list of food items.

## Technical Details
foodsavr uses two microcontrollers. The ESP-32 CAM is used for the camera, but since it doesn't have enough pins, another board is necessary. The other dev board is an off-brand pico-like RP2040 board. For the display, it uses an E-Ink display from WeAct Studio. The matrix keypad is your standard membrane keypad that is in every single electronics kit. It uses a set of lipos for power with USB-C charging. The charger board is TP4056-based.

## Build Guide
1. Insert the battery's JST connector into the charging board.
2. Insert the keypad's cable into its 1x8 header, with the keypad being in the slot on the top case.
3. Put the E-Ink display on the 2x4 header on the PCB.
4. Plop the top case on the display so that the screw holes of the display line up.
5. Use the M3 nut and bolt to secure the display.
6. Finally, fasten the bottom case so that the PCB rests in the tray.

## Motivation
I designed this because I got tired of leaving food at the back of my fridge to rot. Especially, since I'd save my favourite foods for later, and yet forget about them until expiry. This project reminds you before they expire so that you never leave food in the back of your fridge for too long.

## BOM
Go to BOM.csv for the Bill of Materials

## Exploded 3D Model
<img width="839" height="818" alt="Screenshot 2026-06-06 at 11 40 54" src="https://github.com/user-attachments/assets/7c605676-0397-4a29-9125-57ff6d9f36e1" />

## Assembled 3D Model
<img width="905" height="540" alt="Screenshot 2026-06-06 at 11 41 42" src="https://github.com/user-attachments/assets/dd55f1f2-f8a7-4a38-8366-082197c4c82d" />

https://cad.onshape.com/documents/8ee8f266d123a24e5a0ce496/w/151140d60200875ea61c9eed/e/18a74606bdcc9369348646ee?renderMode=0&uiState=6a249579d641e38a22ea1bbf

## PCB
<img width="4096" height="2358" alt="3D_PCB1_2026-06-06" src="https://github.com/user-attachments/assets/98e68af8-8aa9-421a-bbf5-b77f8a298d87" />

## Schematic
<img width="1031" height="431" alt="Screenshot 2026-06-06 at 12 12 31" src="https://github.com/user-attachments/assets/5f7a58aa-4e43-4a2b-b0f1-37d71631165a" />

## Zine Page
![alt text](image.png)
