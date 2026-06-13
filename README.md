# foodsavr
~30% of food in the world is wasted and sent to landfills. foodsavr is a gadget that magnetically attaches to your fridge to remind you what food is close to expiring.

## Usage
To add a food item:
Show your food item to the camera and press the A button. Then, enter the expiry date of the item with the number pad. Finally, confirm by pressing the A button again. Press D to cancel.

Press B and C to cycle up and down through the list of food items.

## Technical Details
foodsavr uses two microcontrollers. The ESP-32 CAM is used for the camera, but since it doesn't have enough pins, another board is necessary. The other dev board is an off-brand pico-like RP2040 board. For the display, it uses an E-Ink display from WeAct Studio. The matrix keypad is your standard membrane keypad that is in every single electronics kit. It uses a set of lipos for power with USB-C charging. The charger board is TP4056-based.

## Build Guide
1. Insert the battery's JST connector into the charging board.
2. Insert the keypad's cable into its 1x8 header, with the keypad being in the slot on the top case.
3. Put the E-Ink display on the 2x4 header on the PCB.
4. Plop the top case on the display so that the screw holes of the display line up.
5. Use the M3 nuts and bolts to secure the display.
6. For the two corners on the right, insert the M3 heatset inserts into the holes.
7. Add a thin layer of cyanoacryalte glue around the bottom of the magnet holes. Then, insert the magnets.
8. Finally, fasten the bottom case so that the PCB rests in the tray with the M3.

## Motivation
I designed this because I got tired of leaving food at the back of my fridge to rot. Especially, since I'd save my favourite foods for later, and yet forget about them until expiry. This project reminds you before they expire so that you never leave food in the back of your fridge for too long.

## BOM
Go to BOM.csv for the Bill of Materials

## Exploded 3D Model
<img width="894" height="933" alt="Screenshot 2026-06-12 at 20 03 19" src="https://github.com/user-attachments/assets/4a6325a6-25e4-485b-a565-14f44fc3bf77" />

## Assembled 3D Model
<img width="1537" height="775" alt="Screenshot 2026-06-12 at 19 54 57" src="https://github.com/user-attachments/assets/acb79698-d6c5-45ed-81d3-88734ded69b3" />

https://cad.onshape.com/documents/8ee8f266d123a24e5a0ce496/w/151140d60200875ea61c9eed/e/18a74606bdcc9369348646ee?renderMode=0&uiState=6a249579d641e38a22ea1bbf

## PCB
<img width="4096" height="2358" alt="3D_PCB1_2026-06-06" src="https://github.com/user-attachments/assets/98e68af8-8aa9-421a-bbf5-b77f8a298d87" />

## Schematic
<img width="1031" height="431" alt="Screenshot 2026-06-06 at 12 12 31" src="https://github.com/user-attachments/assets/5f7a58aa-4e43-4a2b-b0f1-37d71631165a" />

## Zine Page
![alt text](image.png)
