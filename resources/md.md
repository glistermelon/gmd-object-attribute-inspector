| 1   | Object ID                            | **integer**                                     | the ID of the object                                                               |
| 2   | X Position                           | **float**                                       | the X position of the object                                                       |
| 3   | Y Position                           | **float**                                       | the Y position of the object                                                       |
| 4   | Flipped Horizontally                 | **bool**                                        | whether the object is horizontally flipped                                         |
| 5   | Flipped Vertically                   | **bool**                                        | whether the object is vertically flipped                                           |
| 6   | Rotation                             | **float**                                       | the rotation of the objects in degrees, CW is positive, top is 0                   |
| 7   | Red                                  | **integer**                                     | the Red component of the color in a trigger                                        |
| 8   | Green                                | **integer**                                     | the Green component of the color in a trigger                                      |
| 9   | Blue                                 | **integer**                                     | the Blue component of the color in a trigger                                       |
| 10  | Duration / Chance                    | **float**                                       | the duration of an effect in a trigger or the chance in a Random trigger           |
| 11  | Touch Triggered                      | **bool**                                        | the Touch Triggered property of a trigger                                          |
| 12  | Secret Coin ID                       | **integer**                                     | the ID of a Secret Coin                                                            |
| 13  | Special Object Checked               | **bool**                                        | the checked property of some special objects (gamemode, speed, dual portals, etc.) (handles "Preview" property of orbs and pads)|
| 14  | Tint Ground                          | **bool**                                        | the Tint Ground property of the BG Color trigger                                   |
| 15  | Player Color 1                       | **bool**                                        | the Player Color 1 property of any Color trigger                                   |
| 16  | Player Color 2                       | **bool**                                        | the Player Color 2 property of any Color trigger                                   |
| 17  | Blending                             | **bool**                                        | the Blending property of any Color trigger                                         |
| 19  | 1.9 Color Channel ID                 | **integer**                                     | the legacy Color Channel ID property used in 1.9 levels. If set to a valid value, both the Main and Secondary Color Channel ID properties will be ignored. |
| 20  | Editor Layer 1                       | **integer**                                     | the Editor Layer 1 property of the object                                          |
| 21  | Main Color Channel ID                | **integer**                                     | the Main Color Channel ID property of the object                                   |
| 22  | Secondary Color Channel ID           | **integer**                                     | the Secondary Color Channel ID property of the object                              |
| 23  | Target Color ID                      | **integer**                                     | the Target Color ID property in an interactive object                              |
| 24  | Z Layer                              | **integer**                                     | the Z Layer of the object                                                          |
| 25  | Z Order                              | **integer**                                     | the Z Order of the object                                                          |
| 28  | Offset X                             | **integer**                                     | the Offset X property of the Move or camera triggers                               |
| 29  | Offset Y                             | **integer**                                     | the Offset Y property of the Move or camera triggers                               |
| 30  | Easing                               | **integer**                                     | the Easing type of the effect of a trigger                                         |
| 31  | Text                                 | **string**                                      | the text of the text object in [base64]()                                          |
| 32  | Scaling                              | **float**                                       | the scaling of the object                                                          |
| 33  | Single Group ID                      | **integer**                                     | a group ID given to the object                                                      |
| 34  | Group Parent                         | **bool**                                        | the Group Parent property of the object                                            |
| 35  | Opacity                              | **float**                                       | the opacity value of a trigger                                                     |
| 41  | Main Color HSV Enabled               | **bool**                                        | whether the HSV mode is enabled for the Main Color of the object                   |
| 42  | Secondary Color HSV Enabled          | **bool**                                        | whether the HSV mode is enabled for the Secondary Color of the object              |
| 43  | Main Color HSV                       | **[HSV]()**                                     | the HSV adjustment values of the Main Color of the object                          |
| 44  | Secondary Color HSV                  | **[HSV]()**                                     | the HSV adjustment values of the Secondary Color of the object                     |
| 45  | Fade In                              | **float**                                       | the Fade In property of the Pulse trigger                                          |
| 46  | Hold                                 | **float**                                       | the Hold property of the Pulse trigger                                             |
| 47  | Fade Out                             | **float**                                       | the Fade Out property of the Pulse trigger                                         |
| 48  | Pulse Mode                           | **integer**                                     | the Pulse Mode property of the Pulse trigger                                       |
| 49  | Copied Color HSV                     | **[HSV]()**                                     | the HSV adjustment values of the Copied Color property of a trigger                |
| 50  | Copied Color ID                      | **integer**                                     | the Copied Color Channel ID in a trigger                                           |
| 51  | Target Group ID                      | **integer**                                     | the Target Group ID in a trigger                                                   |
| 52  | Pulse Target Type                    | **integer**                                     | the Target Type property of the Pulse trigger                                      |
| 54  | Yellow Teleportation Portal Y Offset | **float**                                       | the Y offset of the yellow from the blue teleportation portal                      |
| 55  | Teleport Portal Ease                 | **bool**                                                 | The Smooth Ease property within Teleport Portals/Orbs                         |
| 56  | Activate Group                       | **bool**                                        | the Activate Group property of the trigger                                         |
| 57  | Group IDs                            | **integer array**                               | the group IDs of the object                                                        |
| 58  | Lock To Player X                     | **bool**                                        | the Lock To Player X property of the Move trigger, or the Don't Zoom X property of the Gameplay Offset trigger       |
| 59  | Lock To Player Y                     | **bool**                                        | the Lock To Player Y property of the Move trigger, or the Don't Zoom Y property of the Gameplay Offset trigger                                  |
| 60  | Copy Opacity                         | **bool**                                        | the Copy Opacity property of a trigger                                             |
| 61  | Editor Layer 2                       | **integer**                                     | the Editor Layer 2 of an object                                                    |
| 62  | Spawn Triggered                      | **bool**                                        | the Spawn Triggered property of a trigger                                          |
| 63  | Spawn Delay                          | **float**                                       | the Spawn Delay property of the Spawn trigger                                      |
| 64  | Don't Fade                           | **bool**                                        | the Don't Fade property of the object                                              |
| 65  | Main Only                            | **bool**                                        | the Main Only property of the Pulse and Area Tint triggers                                        |
| 66  | Detail Only                          | **bool**                                        | the Detail Only property of the Pulse and Area Tint triggers                                      |
| 67  | Don't Enter                          | **bool**                                        | the Don't Enter property of the object                                             |
| 68  | Degrees                              | **integer**                                     | the Degrees property of a rotational trigger                                         |
| 69  | Times 360                            | **integer**                                     | the Times 360 property of the Rotate trigger                                       |
| 70  | Lock Object Rotation                 | **bool**                                        | the Lock Object Rotation property of the Rotate trigger, or the Add property of the Rotate camera trigger |
| 71  | Secondary Group ID                   | **integer**                                     | the Secondary (Follow, Target Pos, Center) Group ID property of some triggers      |
| 72  | X Mod                                | **float**                                       | the X Mod property of the Follow trigger                                           |
| 73  | Y Mod                                | **float**                                       | the Y Mod property of the Follow trigger                                           |
| 75  | Strength                             | **float**                                       | the Strength property of the Shake trigger                                         |
| 76  | Animation ID                         | **integer**                                     | the Animation ID property of the Animate trigger                                   |
| 77  | Count                                | **integer**                                     | the Count property of the Pickup trigger or the Pickup Item                        |
| 78  | Subtract Count                       | **bool**                                        | the Subtract Count property of the Pickup trigger or the Pickup Item               |
| 79  | Pickup Mode                          | **integer**                                     | the Pickup Mode property of the Pickup Item                                        |
| 80  | Item/Block ID                        | **integer**                                     | the Item/Block ID property of an object                                            |
| 81  | Hold Mode                            | **bool**                                        | the Hold Mode property of the Touch trigger                                        |
| 82  | Toggle Mode                          | **integer**                                     | the Toggle Mode property of the Touch trigger                                      |
| 84  | Interval                             | **float**                                       | the Interval property of the Shake trigger                                         |
| 85  | Easing Rate                          | **float**                                       | the Easing Rate property of a trigger                                              |
| 86  | Exclusive                            | **bool**                                        | the Exclusive property of a Pulse trigger                                          |
| 87  | Multi-Trigger                        | **bool**                                        | the Multi-Trigger property of a trigger                                            |
| 88  | Comparison / Pickup Multiply         | **integer**                                     | the Comparison property of the Instant Count trigger / the Multiply/Divide property of the Pickup trigger (1=multiply,2=divide) |
| 89  | Dual Mode                            | **bool**                                        | the Dual Mode property of the Touch trigger                                        |
| 90  | Speed                                | **float**                                       | the Speed property of the Follow Player Y trigger                                  |
| 91  | Follow Delay                         | **float**                                       | the Follow Delay property of the Follow Player Y trigger                           |
| 92  | Y Offset                             | **float**                                       | the Y Offset property of the Follow Player Y trigger                               |
| 93  | Trigger On Exit                      | **bool**                                        | the Trigger On Exit property of the Collision trigger                              |
| 94  | Dynamic Block                        | **bool**                                        | the Dynamic Block property of the Collision block                                  |
| 95  | Block B ID / Item 2 ID               | **integer**                                     | the Block B ID property of the Collision trigger or the "ItemID2" property of the Item Compare trigger |
| 96  | No Glow                              | **bool**                                        | the No Glow property of the object                                                 |
| 97  | Custom Rotation Speed                | **float**                                       | the Custom Rotation Speed property of the rotating object in degrees per second    |
| 98  | Disable Rotation                     | **bool**                                        | the Disable Rotation property of the rotating object                               |
| 99  | Multi Activate (Orbs and Pads)       | **bool**                                        | the Multi Activate property of Orbs and Pads, and the Toggle Block                 |
| 100 | Target/Aim Mode                      | **bool**                                        | the Target Mode property of the Move trigger; the Aim Mode property of the Rotate trigger                                 |
| 101 | Target Pos Coordinates               | **integer**                                     | the Target Pos Coordinates property of the Move or camera triggers                 |
| 102 | Preview Disable                      | **bool**                                        | the Preview Disable property of the Spawn trigger                                  |
| 103 | High Detail                          | **bool**                                        | the High Detail property of the object                                             |
| 104 | Multi Activate (Triggers)            | **bool**                                        | The Multi Activate Property of Triggers                               |
| 105 | Max Speed                            | **float**                                       | the Max Speed property of the Follow Player Y trigger                              |
| 106 | Randomize Start                      | **bool**                                        | the Randomize Start property of the animated object                                |
| 107 | Animation Speed                      | **float**                                       | the Animation Speed property of the animated object                                |
| 108 | Linked Group ID                      | **integer**                                     | the Linked Group ID property of the object                                         |
| 110 | Exit Static                          | **bool**                                        | camera static trigger |
| 111 | Free Mode                            | **bool**                                        | camera mode trigger |
| 112 | Edit Camera Settings                 | **bool**                                        | camera mode trigger |
| 113 | Camera Mode Easing                   | **integer**                                     | camera mode trigger |
| 114 | Camera Padding                       | **float**                                       | camera mode trigger |
| 115 | Order                                | **integer**                                     | order ("ORD")
| 116 | No Effects                           | **bool**                                        | Extra |
| 117 | Reverse                              | **bool**                                        | orbs and pads, but not dash orbs |
| 120 | Time Modifier                        | **float**                                       | timewarp trigger |
| 121 | No Touch                             | **bool**                                        | Extra |
| 122 | Animation Use Speed                  | **bool**                                        | animation |
| 123 | Animate on Trigger                   | **bool**                                        | animation |
| 126 | Animation Disable Delayed Loop       | **bool**                                        | animation |
| 127 | Disable Animation Shine              | **bool**                                        | animation |
| 128 | Scale X                              | **float**                                       | scale X property of an object |
| 129 | Scale Y                              | **float**                                       | scale Y property of an object |
| 131 | Shear X                              | **float**                                       | Always present if Shear Y is present. |
| 132 | Shear Y                              | **float**                                       | Always present if Shear X is present. |
| 133 | Only Move                            | **bool**                                        | scale trigger |
| 134 | Passable                             | **bool**                                        | Extra |
| 135 | Hide                                 | **bool**                                        | Extra |
| 136 | Non Stick X                          | **bool**                                        | Extra |
| 137 | Ice Block                            | **bool**                                        | Extra |
| 138 | Player Pos / P1                      | **bool**                                        | checkpoint / more... |
| 139 | Pickup Override                      | **bool**                                        | pickup trigger |
| 141 | Move Lock X to Camera                | **bool**                                        | move trigger |
| 142 | Move Lock Y to Camera                | **bool**                                        | move trigger |
| 143 | Move Mod X                           | **float**                                       | move trigger, bg/mg speed trigger |
| 144 | Move Mod Y                           | **float**                                       | move trigger, bg/mg speed trigger |
| 148 | Gravity                              | **float**                                       | gravity trigger |
| 149 | Force                                | **float**                                       | The Force property of a Force Block |
| 150 | Scale X                              | **float**                                       | scale trigger |
| 151 | Scale Y                              | **float**                                       | scale trigger |
| 152 | Advanced Random Trigger Data         | **integer array**                               | advanced random trigger, groupId.chance.groupId.chance... etc. |
| 153 | Divide by Value X                    | **bool**                                        | scale trigger |
| 154 | Divide by Value Y                    | **bool**                                        | scale trigger |
| 159 | Additive Streak                      | option (1 = enable, -1 = disable, 0 = neither)  | options trigger |
| 160 | Unlink Dual Gravity                  | option                                          | options trigger |
| 161 | Hide Ground                          | option                                          | options trigger |
| 162 | Hide P1                              | option                                          | options trigger |
| 163 | Hide P2                              | option                                          | options trigger |
| 164 | Camera Edge                          | **integer**                                     | camera edge trigger; 1 = left, 2 = right, 3 = up, 4 = down |
| 165 | Disable P1 Controls                  | **bool**                                        | options trigger |
| 169 | Edit Velocity                        | **bool**                                        | rotate gameplay trigger |
| 170 | Channel                              | **integer**                                     ||
| 171 | Change Channel                       | **bool**                                        | rotate gameplay trigger |
| 172 | Channel Only                         | **bool**                                        | rotate gameplay trigger |
| 173 | Target Channel                       | **integer**                                     | rotate gameplay trigger |
| 175 | Shader Speed                         | **float**                                       | shader triggers |
| 176 | Shader Strength                      | **float**                                       | shader triggers |
| 177 | Shader Time Offset                   | **float**                                       | shader triggers |
| 179 | Wave Line Width                      | **float**                                       | shader triggers |
| 180 | Shader Thickness                     | **float**                                       | shader triggers |
| 181 | Shader Fade In                       | **float**                                       | shader triggers |
| 182 | Shader Fade Out                      | **float**                                       | shader triggers |
| 183 | Inner Shockwave Size                 | **float**                                       | shader triggers |
| 184 | Shader Invert                        | **bool**                                        | shader triggers |
| 188 | No Player Particles                  | **bool**                                        | shader triggers |
| 191 | Outer Shockwave Size                 | **float**                                       | shader triggers |
| 192 | Disable All Shaders                  | **bool**                                        | shader triggers |
| 193 | Grip Slope                           | **bool**                                        | Extra |
| 195 | Hide Middleground                    | **bool**                                        | options trigger |
| 196 | Layer Range Start                    | **integer**                                     | shader triggers (unset = 550084152) |
| 197 | Layer Range End                      | **integer**                                     | shader triggers (unset = 550087896) |
| 198 | Touch P1/P2 Only                     | **integer**                                     | touch trigger; 0 = neither, 1 = P1, 2 = P2 |
| 199 | Disable P2 Controls                  | **bool**                                        | options trigger |
| 200 | (Target/Direction/Aim) P2            | **bool**                                        | move trigger / rotate trigger / more... |
| 201 | Follow Center / PP / PT              | **bool**                                        | (re-target) advanced follow trigger ("C") / (instant) collision trigger ("PP") / gravity trigger ("PT") |
| 210 | Disable Legacy/Static HSV            | **bool**                                        | color & pulse triggers |
| 212 | Static Follow                        | **bool**                                        | camera static trigger |
| 213 | Static Camera Easing                 | **float**                                       | camera static trigger |
| 214 | Animation Only if Active             | **bool**                                        | animation |
| 218 | Area Move Distance                   | **integer**                                     | (edit) area move trigger |
| 219 | Area Move Distance Variance          | **integer**                                     | (edit) area move trigger |
| 220 | Area Move Offset                     | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 221 | Area Move Offset Variance            | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 222 | Area Move Length                     | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 223 | Area Move Length Variance            | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 225 | Area Move Effect ID                  | **integer**                                     | area move/rotate/scale/fade/tint trigger |
| 231 | Area Move Angle                      | **integer**                                     | (edit) area move trigger |
| 232 | Area Move Angle Variance             | **integer**                                     | (edit) area move trigger |
| 233 | Area Scale X                         | **float**                                       | (edit) area scale trigger; -99 for edit area move trigger |
| 234 | Area Scale X Variance                | **float**                                       | (edit) area scale trigger; -99 for edit area move trigger |
| 235 | Area Scale Y                         | **float**                                       | (edit) area scale trigger; -99 for edit area move trigger |
| 236 | Area Scale Y Variance                | **float**                                       | (edit) area scale trigger; -99 for edit area move trigger |
| 237 | Area Move X                          | **integer**                                     | (edit) area move trigger; -99 for edit area move trigger |
| 238 | Area Move X Variance                 | **integer**                                     | (edit) area move trigger; -99 for edit area move trigger |
| 239 | Area Move Y                          | **integer**                                     | (edit) area move trigger; -99 for edit area move trigger |
| 240 | Area Move Y Variance                 | **integer**                                     | (edit) area move trigger; -99 for edit area move trigger |
| 241 | XY Mode Enabled                      | **bool**                                        | (edit) area move trigger |
| 248 | Outward Easing                       | **integer**                                     | area move trigger |
| 252 | Area Move Offset Y                   | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 253 | Area Move Offset Y Variance          | **integer**                                     | (edit) area move/rotate/scale/fade/tint trigger |
| 260 | Area Tint Color Channel              | **integer**                                     | area tint trigger |
| 261 | Enable Ease Out                      | **bool**                                        | area move trigger |
| 262 | Area Move Direction                  | **integer**                                     | area move/rotate/scale/fade/tint trigger; THIS IS AN ENUM! 1, 2; AREA MOVE NOTES |
| 263 | Area Move Mod Front                  | **float**                                       | (edit) area move/rotate/scale/fade/tint trigger |
| 264 | Area Move Mod Back                   | **float**                                       | (edit) area move/rotate/scale/fade/tint trigger |
| 265 | Area Tint                            | **float**                                       | (edit) area tint trigger; -99 for edit area move trigger |
| 270 | Area Rotation                        | **float**                                       | (edit) area rotate trigger; -99 for edit area move trigger |
| 271 | Area Rotation Variance               | **float**                                       | (edit) area rotate trigger; -99 for edit area move trigger |
| 275 | Area Fade To Opacity                 | **float**                                       | (edit) area fade trigger; -99 for edit area move trigger |
| 276 | Area Move Mode Discriminator         | **bool**                                        | area move/rotate/scale/fade/tint trigger; AREA MOVE NOTES |
| 278 | Area Tint Use HSV                    | **bool**                                        | (edit) area tint trigger |
| 279 | Area Parent                          | **bool**                                        | Extra |
| 280 | Ignore Group Parent                  | **bool**                                        | area move/rotate/scale/fade/tint trigger |
| 281 | Ignore Linked                        | **bool**                                        | area move/rotate/scale/fade/tint trigger |
| 282 | Area Move Deadzone                   | **float**                                       | (edit) area move/rotate/scale/fade/tint trigger |
| 283 | Area Move Bidirectionally            | **bool**                                        | area move/rotate/scale/fade/tint trigger; AREA MOVE NOTES |
| 284 | Single P-Touch                       | **bool**                                        | orbs and pads |
| 286 | Area Fade From Opacity               | **float**                                       | area fade trigger; -99 for edit area move trigger |
| 287 | Area Move Relative                   | **bool**                                        | area move trigger |
| 288 | Area Move Relative Fade              | **float**                                       | (edit) area move trigger |
| 289 | Non Stick Y                          | **bool**                                        | Extra |
| 290 | Shader Screen Offset X               | **float**                                       | shader triggers |
| 291 | Shader Screen Offset Y               | **float**                                       | shader triggers |
| 292 | Follow Delay                         | **float**                                       | advanced follow trigger |
| 293 | Follow Delay Variance                | **float**                                       | advanced follow trigger |
| 298 | Follow Max Speed                     | **float**                                       | advanced follow trigger |
| 299 | Follow Max Speed Variance            | **float**                                       | advanced follow trigger |
| 300 | Follow Start Speed                   | **float**                                       | (edit) advanced follow trigger |
| 301 | Follow Start Speed Variance          | **float**                                       | (edit) advanced follow trigger |
| 305 | Follow Target Direction              | **bool**                                        | advanced follow trigger |
| 306 | Follow X Only                        | **bool**                                        | (edit) advanced follow trigger |
| 307 | Follow Y Only                        | **bool**                                        | (edit) advanced follow trigger |
| 308 | Follow Max Range                     | **integer**                                     | advanced follow trigger |
| 309 | Follow Max Range Variance            | **integer**                                     | advanced follow trigger |
| 316 | Follow Steer Force                   | **float**                                       | advanced follow trigger |
| 317 | Follow Steer Force Variance          | **float**                                       | advanced follow trigger |
| 318 | Follow Steer Force Low               | **float**                                       | advanced follow trigger |
| 319 | Follow Steer Force Low Variance      | **float**                                       | advanced follow trigger |
| 320 | Follow Steer Force High              | **float**                                       | advanced follow trigger |
| 321 | Follow Steer Force High Variance     | **float**                                       | advanced follow trigger |
| 322 | Follow Speed Range Low               | **float**                                       | advanced follow trigger |
| 323 | Follow Speed Range Low Variance      | **float**                                       | advanced follow trigger |
| 324 | Follow Speed Range High              | **float**                                       | advanced follow trigger |
| 325 | Follow Speed Range High Variance     | **float**                                       | advanced follow trigger |
| 328 | Follow Break Angle                   | **integer**                                     | advanced follow trigger |
| 329 | Follow Break Angle Variance          | **integer**                                     | advanced follow trigger |
| 330 | Follow Break Steer Force             | **float**                                       | advanced follow trigger |
| 331 | Follow Break Steer Force Variance    | **float**                                       | advanced follow trigger |
| 332 | Follow Break Steer Speed Limit       | **float**                                       | advanced follow trigger |
| 333 | Follow Break Steer Speed Limit Variance | **float**                                    | advanced follow trigger |
| 334 | Follow Acceleration                  | **float**                                       | advanced follow trigger |
| 335 | Follow Acceleration Variance         | **float**                                       | advanced follow trigger | 
| 336 | Ignore Disabled                      | **bool**                                        | advanced follow trigger |
| 337 | Enable Low Speed Mode                | **bool**                                        | advanced follow trigger |
| 338 | Enable High Speed Mode               | **bool**                                        | advanced follow trigger |
| 339 | Follow Rotate Directional            | **bool**                                        | advanced follow trigger |
| 340 | Follow Rotation Offset               | **integer**                                     | advanced follow trigger |     
| 341 | Area Move Priority                   | **integer**                                     | area move/rotate/scale/fade/tint trigger |                 
| 343 | Enter Channel                        | **integer**                                     | Extra2 |
| 345 | Static Rotate Force                  | **bool**                                        | teleport |
| 346 | Rotate Force                         | **float**                                       | teleport |
| 347 | Redirect Rotate Force                | **bool**                                        | teleport |
| 348 | Redirect Rotate Force Minimum        | **float**                                       | teleport |
| 349 | Redirect Rotate Force Maximum        | **float**                                       | teleport |
| 350 | Redirect Rotate Force Mod            | **float**                                       | teleport |
| 351 | Save Offset                          | **bool**                                        | teleport |
| 352 | Ignore X                             | **bool**                                        | teleport |
| 353 | Ignore Y                             | **bool**                                        | teleport |
| 354 | Teleport Gravity                     | **integer**                                     | The gravity setting of the teleport portals/orbs (1 = Normal, 2 = Flipped, 3 = Toggle) |
| 355 | Use Effect ID                        | **bool**                                        | edit area move trigger |
| 357 | Follow Near Acceleration             | **float**                                       | advanced follow trigger |
| 358 | Follow Near Acceleration Variance    | **float**                                       | advanced follow trigger |
| 359 | Follow Near/Slow Distance            | **float**                                       | advanced follow trigger |
| 360 | Follow Near/Slow Distance Variance   | **float**                                       | advanced follow trigger |
| 361 | Follow Easing                        | **float**                                       | advanced follow trigger |
| 362 | Follow Easing Variance               | **float**                                       | advanced follow trigger |
| 356 | Scale Stick                          | **bool**                                        | Extra |
| 363 | Follow Rotate Easing                 | **integer**                                     | advanced follow trigger |
| 364 | Follow Rotate Deadzone               | **integer**                                     | advanced follow trigger |
| 365 | Follow Priority                      | **integer**                                     | advanced follow trigger |
| 366 | Follow Max Range Extra Option        | **integer**                                     | advanced follow trigger |
| 368 | Instant Offset                       | **bool**                                        | gameplay rotate trigger |
| 369 | Center Effect                        | **bool**                                        | orbs and pads |
| 370 | Disable Grid Snap                    | **bool**                                        | camera mode trigger |
| 371 | Zoom Factor                          | **float**                                       | camera zoom/guide triggers |
| 372 | No Scale Audio                       | **bool**                                        | Extra |
| 375 | Keyframe Reference Only              | **bool**                                        | keyframe trigger |
| 376 | Close Loop                           | **bool**                                        | keyframe trigger |
| 377 | Keyframe Proximity                   | **bool**                                        | keyframe trigger |
| 378 | Keyframe Curve                       | **bool**                                        | keyframe trigger |
| 379 | Keyframe Trigger Mode                | **integer**                                     | keyframe trigger; 0 = time, 1 = even, 2 = dist |
| 380 | Preview Art                          | **bool**                                        | keyframe trigger |
| 381 | Pickup Item                          | **bool**                                        | pickup item |
| 382 | Pickup Item Toggle Trigger           | **bool**                                        | pickup item |
| 383 | Pickup Item Points                   | **integer**                                     | pickup item |
| 385 | UI X Reference                       | **integer**                                     | ui trigger; 0 | 1 = auto, 2 = center, 3 = left, 4 = right |
| 386 | UI Y Reference                       | **integer**                                     | ui trigger; 0 | 5 = auto, 6 = center, 7 = left, 8 = right |
| 387 | UI X Relative                        | **bool**                                        | ui trigger |
| 388 | UI Y Relative                        | **bool**                                        | ui trigger |
| 389 | Counter Seconds Only                 | **bool**                                        | counter |
| 390 | Special Counter Mode                 | **integer**                                     | counter; 0 = none, -1 = maintime, -2 = points, -3 = attempts |
| 391 | Counter Alignment                    | **integer**                                     | counter; 0 = none, 1 = left, 2 = right |
| 392 | Soundtrack ID                        | **integer**                                     | song trigger, sfx trigger |
| 393 | Small Step                           | **bool**                                        | move trigger |
| 394 | Direction/Follow Mode/Snap360        | **bool**                                        | move & rotate triggers + camera rotate trigger |
| 395 | Center Group ID                      | **integer**                                     | move trigger |
| 396 | Directional Distance                 | **integer**                                     | move trigger |
| 397 | Dynamic Mode                         | **bool**                                        | move & rotate triggers |
| 399 | Prep Song                            | **bool**                                        | song trigger |
| 400 | Load Prep Song                       | **bool**                                        | song trigger |
| 401 | Rotation Target ID                   | **integer**                                     | rotate trigger |
| 402 | Rotation Offset                      | **float**                                       | rotate trigger |
| 404 | Sound Speed                          | **integer**                                     | (edit) song/sfx trigger |
| 405 | Pitch                                | **integer**                                     | sfx trigger |
| 406 | Volume                               | **float**                                       | (edit) song/sfx trigger |
| 407 | Enable Reverb                        | **bool**                                        | (edit) sfx trigger |
| 408 | Song Start                           | **integer**                                     | song trigger, sfx trigger |
| 409 | Song Fade In                         | **integer**                                     | song trigger, sfx trigger |
| 410 | Song End                             | **integer**                                     | song trigger, sfx trigger |
| 411 | Song Fade Out                        | **integer**                                     | song trigger, sfx trigger |
| 412 | FFT                                  | **bool**                                        | sfx trigger |
| 413 | Loop Song                            | **bool**                                        | song trigger, sfx trigger |
| 414 | Stop Sound Loop                      | **bool**                                        | edit song/sfx trigger |
| 415 | SFX Is Unique                        | **bool**                                        | sfx trigger |
| 416 | SFX Unique ID                        | **integer**                                     | (edit) sfx trigger |
| 417 | Stop Sound                           | **bool**                                        | edit song/sfx trigger |
| 418 | Change Volume                        | **bool**                                        | edit song/sfx trigger |
| 419 | Change Song Speed                    | **bool**                                        | edit song/sfx trigger |
| 420 | SFX Override                         | **bool**                                        | sfx trigger |
| 421 | Volume Near                          | **float**                                       | edit song trigger, (edit) sfx trigger |
| 422 | Volume Medium                        | **float**                                       | edit song trigger, (edit) sfx trigger |
| 423 | Volume Far                           | **float**                                       | edit song trigger, (edit) sfx trigger |
| 424 | Sound Minimum Distance               | **integer**                                     | edit song trigger, (edit) sfx trigger |
| 425 | Sound Distance 2                     | **integer**                                     | edit song trigger, (edit) sfx trigger |
| 426 | Sound Distance 3                     | **integer**                                     | edit song trigger, (edit) sfx trigger |
| 428 | Use Camera                           | **bool**                                        | edit song trigger, (edit) sfx trigger: "Cam" |
| 430 | Linked Events                        | **integer array**                               | event link trigger; ARRAY OF ENUMS |
| 432 | Song Channel                         | **integer**                                     | (edit) song trigger |
| 433 | Pre-Load SFX                         | **bool**                                        | sfx trigger |
| 434 | SFX Minimum Interval                 | **float**                                       | sfx trigger |
| 435 | Sequence Data                        | **integer array**                               | sequence trigger; groupId.count.groupId.count... etc. |
| 436 | Sequence Mode                        | **integer**                                     | sequence trigger; 0 = stop, 1 = loop, 2 = last |
| 437 | Minimum Interval                     | **float**                                       | sequence trigger |
| 438 | Sequence Reset                       | **float**                                       | sequence trigger |
| 439 | Sequence Reset Mode                  | **bool**                                        | sequence trigger; 0 = full, 1 = step |
| 440 | Pickup Item Particle                 | **integer**                                     | pickup item |
| 441 | Spawn Ordered                        | **bool**                                        | spawn trigger |
| 442 | Spawn Group ID Remapping             | **integer array**                               | spawn trigger |
| 443 | Static Rotate Force Additive         | **bool**                                        | teleport |
| 445 | Claim Touch                          | **bool**                                        | trigger orb, toggle block |
| 446 | Material                             | **integer**                                     | Extra2 |
| 447 | Extra ID                             | **integer**                                     | event link trigger |
| 448 | Respawn ID                           | **integer**                                     | checkpoint |
| 449 | Pickup Modifier                      | **float**                                       | pickup trigger |
| 452 | Relative Rotation                    | **bool**                                        | scale trigger |
| 453 | Smooth Velocity                      | **bool**                                        | camera static trigger |
| 454 | Camera Velocity Modifier             | **float**                                       | camera static trigger |
| 455 | SFX Group                            | **integer**                                     | (edit) sfx trigger |
| 458 | Sound Effect Mode                    | **integer**                                     | edit sound trigger, (edit) sfx trigger; enum, matches the buttons (0,1,2,3,4,5,6) (read like a book) |
| 459 | Auto Layer                           | **bool**                                        | keyframe trigger |
| 460 | No Effects on End                    | **bool**                                        | end trigger |
| 461 | No SFX on End                        | **bool**                                        | end trigger |
| 462 | Animation Single Frame               | **integer**                                     | animation |
| 463 | Pickup Item No Animation             | **bool**                                        | pickup item |
| 464 | Instant Camera                       | **bool**                                        | teleport |
| 465 | Exit Instant                         | **bool**                                        | camera static trigger |
| 466 | Time Counter                         | **bool**                                        | counter |
| 467 | Start Time                           | **float**                                       | time trigger |
| 468 | Don't Override Time                  | **bool**                                        | time trigger |
| 469 | Ignore Time Warp                     | **bool**                                        | time trigger |
| 470 | Time Mod                             | **float**                                       | time trigger |
| 471 | Start Paused                         | **bool**                                        | time trigger |
| 472 | Time Control Stop                    | **bool**                                        | time control trigger |
| 473 | Stop/Target Time                     | **float**                                       | time / time event trigger |
| 474 | Enable Stop Time                     | **bool**                                        | time trigger |
| 475 | Time Event Multi-Active              | **bool**                                        | time event trigger |
| 476 | Item ID 1                            | **integer**                                     | item edit/compare trigger; 1 = item, 2 = timer, 3 = points, 4 = time, 5 = att |
| 477 | Item ID 2                            | **integer**                                     | item edit/compare trigger; 1 = item, 2 = timer, 3 = points, 4 = time, 5 = att |
| 479 | Item Mod                             | **float**                                       | item edit/compare trigger |
| 480 | Item Edit Arithmetic #1              | **integer**                                     | item edit/compare trigger; 1 = +, 2 = -, 3 = *, 4 = / |
| 481 | Item Edit Arithmetic #2              | **integer**                                     | item edit/compare trigger; 1 = +, 2 = -, 3 = *, 4 = / |
| 482 | Item Edit Arithmetic #3              | **integer**                                     | item edit/compare trigger; 0 = ==, 1 = >, 2 = < or >=, 3 = * or <, 4 = / or <=, 5 = != |
| 483 | Item Mod 2                           | **float**                                       | item compare trigger |
| 484 | Item Comparison Variance             | **float**                                       | item compare trigger, "Tol+-" |
| 485 | Item Edit Inner Function #1          | **integer**                                     | item edit/compare trigger; 1 = round, 2 = floor, 3 = ceiling |
| 486 | Item Edit Inner Function #2          | **integer**                                     | item edit/compare trigger; 1 = round, 2 = floor, 3 = ceiling |
| 487 | Instant End                          | **bool**                                        | end trigger |
| 488 | Text Kerning                         | **integer**                                     | text |
| 489 | Ignore Volume Test                   | **bool**                                        | sfx trigger |
| 491 | Persist Item                         | **bool**                                        | item pers trigger |
| 492 | Target All                           | **bool**                                        | item pers trigger |
| 493 | Item Reset                           | **bool**                                        | item pers trigger |
| 494 | Item Timer                           | **bool**                                        | item pers trigger |
| 495 | Extra Sticky                         | **bool**                                        | Extra |
| 496 | Don't Boost Y                        | **bool**                                        | Extra |
| 498 | Beats per Minute                     | **integer**                                     | bpm trigger |
| 499 | BPM Speed                            | **integer**                                     | bpm trigger; 0 = blue, 1 = yellow, 2 = green, 3 = pink, 4 = red |
| 500 | Disable BPM                          | **bool**                                        | bpm trigger |
| 501 | Beats per Bar                        | **integer**                                     | bpm trigger |
| 502 | New Global Reverb                    | **integer**                                     | sfx trigger; ENUM |
| 503 | Edit Global Reverb                   | **bool**                                        | sfx trigger |
| 504 | Spawn Only                           | **bool**                                        | trigger orb, toggle block |
| 505 | Unique Remap                         | **bool**                                        | sequence trigger |
| 506 | Preview Opacity                      | **float**                                       | camera guide trigger |
| 507 | No Particle                          | **bool**                                        | Extra |
| 509 | Don't Boost X                        | **bool**                                        | Extra |
| 510 | Snap Ground                          | **bool**                                        | teleport |
| 511 | Extended Collision                   | **bool**                                        | Extra |
| 512 | Shader Maximum Size                  | **float**                                       | shader triggers |
| 516 | Aim Minimum X ID                     | **integer**                                     | rotate trigger |
| 517 | Aim Minimum Y ID                     | **integer**                                     | rotate trigger |
| 518 | Aim Maximum X ID                     | **integer**                                     | rotate trigger |
| 519 | Aim Maximum Y ID                     | **integer**                                     | rotate trigger |
| 520 | Keyframe Time Mod                    | **float**                                       | keyframe |
| 521 | Keyframe Position X Mod              | **float**                                       | keyframe |
| 522 | Keyframe Rotation Mod                | **float**                                       | keyframe |
| 523 | Keyframe Scale X Mod                 | **float**                                       | keyframe |
| 525 | Extra ID 2                           | **integer**                                     | event link trigger |
| 526 | Minimum Force                        | **float**                                       | force block |
| 527 | Maximum Force                        | **float**                                       | force block |
| 528 | Relative Force                       | **bool**                                        | force block |
| 529 | Range Force                          | **bool**                                        | force block |
| 530 | Force ID                             | **integer**                                     | force block |
| 531 | Mysterious Shader Checkbox           | **bool**                                        | shader triggers |
| 532 | Hide Attempts                        | option                                          | options trigger |
| 533 | Background/Ground/Middleground ID    | **integer**                                     | background/ground/middleground (BG/G/MG) triggers |
| 534 | Control ID                           | **integer**                                     | Extra2, only some objects: orbs, pads, portals, triggers, pickup items |
| 535 | Use Control ID                       | **bool**                                        | stop & edit advanced follow triggers |
| 536 | Keyframe Clockwise                   | **integer**                                     | keyframe trigger; 0 = default, 1 = clockwise, 2 = counterclockwise |
| 537 | Keyframe Times 360                   | **integer**                                     | keyframe trigger |
| 538 | Area Move Special Center Group ID    | **integer**                                     | area move/rotate/scale/fade/tint trigger; from -1 to -11: P1, P2, C, BL, CL, TL, BC, TC, BR, CR, TR |
| 539 | Don't Edit Area Parent               | **bool**                                        | area move/rotate/scale/fade/tint trigger ("DE AP") |
| 540 | Stop Player Jump                     | **bool**                                        | player control trigger |
| 541 | Stop Player Move                     | **bool**                                        | player control trigger |
| 542 | Stop Player Rotation                 | **bool**                                        | player control trigger |
| 543 | Stop Player Slide                    | **bool**                                        | player control trigger |
| 544 | Move Silent                          | **bool**                                        | move trigger |
| 545 | Keyframe Position Y Mod              | **float**                                       | keyframe |
| 546 | Keyframe Scale Y Mod                 | **float**                                       | keyframe |
| 547 | Particle Offset X                    | **integer**                                     | spawn particle trigger |
| 548 | Particle Offset Y                    | **integer**                                     | spawn particle trigger |
| 549 | Particle Offset X Variance           | **integer**                                     | spawn particle trigger |
| 550 | Particle Offset Y Variance           | **integer**                                     | spawn particle trigger |
| 551 | Particle Match Rotation              | **bool**                                        | spawn particle trigger |
| 552 | Particle Rotation                    | **integer**                                     | spawn particle trigger |
| 553 | Particle Rotation Variance           | **integer**                                     | spawn particle trigger |
| 554 | Particle Scale                       | **float**                                       | spawn particle trigger |
| 555 | Particle Scale Variance              | **float**                                       | spawn particle trigger |
| 556 | Spawn Delay Mod                      | **float**                                       | spawn trigger |
| 557 | Keyframe Spawn Delay                 | **float**                                       | keyframe trigger |
| 558 | Follow Friction                      | **float**                                       | advanced follow trigger |
| 559 | Follow Friction Variance             | **float**                                       | advanced follow trigger |
| 560 | Follow Start Speed Extra Option      | **integer**                                     | (edit) advanced follow trigger |
| 561 | Follow Near Friction                 | **float**                                       | advanced follow trigger |
| 562 | Follow Near Friction Variance        | **float**                                       | advanced follow trigger |
| 563 | Follow Start Direction               | **integer**                                     | (edit) advanced follow trigger |
| 564 | Follow Start Direction Variance      | **integer**                                     | (edit) advanced follow trigger |
| 565 | Follow Start Direction Extra Option  | **integer**                                     | (edit) advanced follow trigger |
| 566 | Follow Mod X                         | **float**                                       | edit (& re-target) advanced follow trigger |
| 567 | Follow Mod X Variance                | **float**                                       | edit advanced follow trigger |
| 568 | Follow Mod Y                         | **float**                                       | edit (& re-target) advanced follow trigger |
| 569 | Follow Mod Y Variance                | **float**                                       | edit advanced follow trigger |
| 570 | Redirection Follow Direction         | **bool**                                        | edit advanced follow trigger |
| 571 | Follow Exclusive                     | **bool**                                        | advanced follow trigger |
| 572 | Follow Mode                          | **integer**                                     | advanced follow trigger; 0 = init, 1 = set, 2 = add |
| 573 | Edit Respawn Time                    | **bool**                                        | options trigger |
| 574 | Respawn Time                         | **float**                                       | options trigger |
| 575 | Audio on Death                       | option                                          | options trigger |
| 576 | No Death SFX                         | option                                          | options trigger |
| 577 | Relative Scale                       | **bool**                                        | scale trigger |
| 578 | Item Edit Outer Function #1          | **integer**                                     | item edit/compare trigger; 1 = abs, 2 = neg |
| 579 | Item Edit Outer Function #2          | **integer**                                     | item edit/compare trigger; 1 = abs, 2 = neg |
| 580 | Pause or Resume                      | **integer**                                     | stop trigger; 1 = pause, 2 = resume |
| 581 | Reset Remap                          | **bool**                                        | spawn trigger |
| 582 | Velocity X Modifier                  | **float**                                       | rotate gameplay trigger |
| 583 | Velocity Y Modifier                  | **float**                                       | rotate gameplay trigger |
| 584 | Override Velocity                    | **bool**                                        | rotate gameplay trigger |
| 585 | Don't Slide                          | **bool**                                        | rotate gameplay trigger |
| 591 | Redirect Dash                        | **bool**                                        | teleport |
| 592 | Offset Animation                     | **bool**                                        | animation |
| 593 | Boost Slide                          | option                                          | options trigger |