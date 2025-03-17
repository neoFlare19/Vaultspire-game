#include "iGraphics.h"
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

// Structs and variables for the new level
int logoX = SCREEN_WIDTH / 14;
int logoY = SCREEN_HEIGHT / 10;
int logoWidth = 50;
int logoHeight = 50;
int dx = 0;
int dy = 0;
int boxSpeed = 20; // Increased box speed
int backgroundImage; // Variable to store the loaded image ID

// Structs and variables for the fighting stage
struct MenuOption {
	int x, y;
	const char* text;
	bool isClicked(int mx, int my) {
		return (mx >= x && mx <= x + 200 && my >= y - 20 && my <= y + 10);
	}
};

struct Spell {
	int x, y;
	bool active;
	float angle; // For spell rotation
	bool isSpecial; // Flag for special attack
};

struct HealthKit {
	int x, y;
	bool active;
};

MenuOption menu[5] = {
	{ 500, 400, "Play Game" },
	{ 500, 350, "Instructions" },
	{ 500, 300, "About Us" },
	{ 500, 250, "Sound:ON/OFF" },
	{ 500, 200, "Exit" }
};

int winScreenID, overScreenID; // Variables to store win and over screen images

int levelBackgroundID, characterFrames[4], spell1ID, spell2ID, spell3ID, spell5ID, spell6ID, healthKitID;
int villainFrames[5], bossFrames[7];
int currentVillainFrame = 0;
int currentCharacterFrame = 0;
int currentBossFrame = 0;

int characterX = 100, characterY = 100;
int characterSpeed = 10;
int villainX = 800, villainY = 100;
int villainSpeed = 10;
int bossSpeed = 15;
bool villainDirection = true;
bool villainVisible = true;

std::vector<Spell> playerSpells;
std::vector<Spell> villainSpells;
HealthKit healthKit = { -100, -100, false };

int mainMenuBackgroundID, aboutUsBackgroundID, startingScreenID, playGameScreenID, secondImageID, thirdImageID;
int newStageBackgroundID; // Background for the new stage (currentPage == 7)
int instructionScreenID; // Instruction screen image ID
int currentPage = -1; // -1: Starting Screen, 0: Main Menu, 1: Play Game Screen, 2: Second Image, 3: New Level, 4: Fighting Stage, 5: Third Stage, 6: Game Over, 7: New Stage After Boss Defeat, 8: About Us, 9: Win Screen, 10: Instructions
int selectedOption = -1;
int villainKillCount = 0;
char killText[20] = "Kills: 0";

int backgroundX = 0;
const int backgroundSpeed = 2;

int characterHealth = 100;
int hitsTaken = 0;
char healthText[20] = "Health: 100%";

bool soundOn = true;

// Animation variables
int flyID; // Image for flying
int attackFrames[3]; // Array for attack animation frames
int currentAttackFrame = 0;
bool isAttacking = false;
int attackAnimationTimer = 0;

// Special attack variables
bool isSpecialAttackReady = true;
int specialAttackCooldown = 0;
const int SPECIAL_ATTACK_COOLDOWN_TIME = 100; // Cooldown in frames (5 seconds at 60 FPS)
int specialAttackID; // Image for special attack

// Boss special movement variables
bool isCharging = false; // Tracks if the boss is charging at the player
int chargeCooldown = 0; // Cooldown for charging
const int CHARGE_COOLDOWN_TIME = 200; // Cooldown time in frames (e.g., 200 frames = ~3.3 seconds at 60 FPS)

bool isZigzagging = false; // Tracks if the boss is moving in a zigzag pattern
int zigzagDirection = 1; // Direction for zigzag movement (1 for right, -1 for left)

// Boss health variables
int bossHealth = 100; // Boss's initial health
const int BOSS_MAX_HEALTH = 100; // Maximum health of the boss
int bossRegenerationTimer = 0; // Timer for health regeneration
const int BOSS_REGENERATION_INTERVAL = 900; // 15 seconds (900 frames at 60 FPS)

// New stage enemy variables
int enemyAttackFrames[5]; // Enemy attack animation frames
int enemyDeadFrames[3]; // Enemy death animation frames
int currentEnemyAttackFrame = 0;
int currentEnemyDeadFrame = 0;
bool isEnemyAttacking = false;
bool isEnemyDead = false;
int enemyAttackTimer = 0;
int enemyDeadTimer = 0;
int enemyX = 800, enemyY = 100;
int enemySpeed = 20;
bool enemyVisible = true;
// Increase enemy health
const int ENEMY_MAX_HEALTH = 100; // Increased from 100 to 500
int enemyHealth = ENEMY_MAX_HEALTH; // Start with full health
int enemyRegenerationTimer = 0; // Timer for enemy health regeneration
const int ENEMY_REGENERATION_INTERVAL = 300; // 5 seconds (300 frames at 60 FPS)
int enemySpecialAttackCooldown = 0; // Timer for enemy special attack
const int ENEMY_SPECIAL_ATTACK_INTERVAL = 600; // 10 seconds (600 frames at 60 FPS)

// Health regeneration variables
int healthRegenerationTimer = 0; // Timer for health regeneration
const int HEALTH_REGENERATION_INTERVAL = 300; // 5 seconds (300 frames at 60 FPS)

void playBackgroundMusic() {
	if (soundOn) {
		PlaySound("Harry_Potter_Theme_Song_Hedwigs_Theme (1).wav", NULL, SND_LOOP | SND_ASYNC);
	}
	else {
		PlaySound(NULL, 0, SND_ASYNC);
	}
}

void updateSpells();
void villainAI();
void bossAI();
void spawnHealthKit();
void updateHealthKit();
void enemyAI();

void drawMenu() {
	for (int i = 0; i < 5; i++) {
		if (i == selectedOption) {
			iSetColor(255, 255, 0);
		}
		else {
			iSetColor(255, 255, 255);
		}
		iText(menu[i].x, menu[i].y, (char*)menu[i].text, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}

void drawRotatedImage(int x, int y, int width, int height, int imageID, float angle) {
	glPushMatrix();
	glTranslatef(x + width / 2, y + height / 2, 0);
	glRotatef(angle, 0, 0, 1);
	glTranslatef(-(x + width / 2), -(y + height / 2), 0);
	iShowImage(x, y, width, height, imageID);
	glPopMatrix();
}

void drawBossHealthBar() {
	if (currentPage == 5) { // Only draw in the final stage
		int barWidth = 200; // Total width of the health bar
		int barHeight = 10; // Height of the health bar
		int barX = villainX; // X position of the health bar (aligned with the boss)
		int barY = villainY + 160; // Y position of the health bar (above the boss)

		// Draw the background of the health bar (empty part)
		iSetColor(100, 100, 100); // Gray color for the background
		iFilledRectangle(barX, barY, barWidth, barHeight);

		// Draw the current health (filled part)
		int currentHealthWidth = (bossHealth * barWidth) / BOSS_MAX_HEALTH; // Scale health to bar width
		iSetColor(255, 0, 0); // Red color for the health bar
		iFilledRectangle(barX, barY, currentHealthWidth, barHeight);

		// Draw the border of the health bar
		iSetColor(255, 255, 255); // White color for the border
		iRectangle(barX, barY, barWidth, barHeight);
	}
}

void drawEnemyHealthBar() {
	if (currentPage == 7) { // Only draw in the new stage
		int barWidth = 200; // Total width of the health bar
		int barHeight = 10; // Height of the health bar
		int barX = enemyX; // X position of the health bar (aligned with the enemy)
		int barY = enemyY + 300; // Y position of the health bar (above the enemy)

		// Draw the background of the health bar (empty part)
		iSetColor(100, 100, 100); // Gray color for the background
		iFilledRectangle(barX, barY, barWidth, barHeight);

		// Draw the current health (filled part)
		int currentHealthWidth = (enemyHealth * barWidth) / ENEMY_MAX_HEALTH; // Scale health to bar width
		iSetColor(255, 0, 0); // Red color for the health bar
		iFilledRectangle(barX, barY, currentHealthWidth, barHeight);

		// Draw the border of the health bar
		iSetColor(255, 255, 255); // White color for the border
		iRectangle(barX, barY, barWidth, barHeight);

		// Display health percentage
		char healthPercentageText[20];
		sprintf_s(healthPercentageText, sizeof(healthPercentageText), "%d%%", enemyHealth);
		iSetColor(255, 255, 255); // White color for the text
		iText(barX + barWidth + 10, barY, healthPercentageText, GLUT_BITMAP_HELVETICA_12);
	}
}

void drawLevel() {
	if (currentPage == 4) { // 2nd stage
		iShowImage(backgroundX, 0, 1280, 720, levelBackgroundID);
		iShowImage(backgroundX + 1280, 0, 1280, 720, levelBackgroundID);
	}
	else if (currentPage == 5) { // Final stage
		iShowImage(backgroundX, 0, 1280, 720, thirdImageID);
		iShowImage(backgroundX + 1280, 0, 1280, 720, thirdImageID);
	}
	else if (currentPage == 7) { // New stage after boss defeat
		iShowImage(backgroundX, 0, 1280, 720, newStageBackgroundID); // Draw the new stage background
		iShowImage(backgroundX + 1280, 0, 1280, 720, newStageBackgroundID); // Draw the new stage background (scrolling)
	}

	// Draw character with appropriate animation
	if (isAttacking) {
		iShowImage(characterX, characterY, 100, 100, attackFrames[currentAttackFrame]); // Attack animation
	}
	else if (characterY > 100) {
		iShowImage(characterX, characterY, 100, 100, flyID); // Flying animation
	}
	else {
		iShowImage(characterX, characterY, 100, 100, characterFrames[currentCharacterFrame]); // Walking animation
	}

	if (villainVisible) {
		if (currentPage == 4) { // 2nd stage
			iShowImage(villainX, villainY, 100, 100, villainFrames[currentVillainFrame]);
		}
		else if (currentPage == 5) { // Final stage
			iShowImage(villainX, villainY, 150, 150, bossFrames[currentBossFrame]);
			drawBossHealthBar(); // Draw the boss's health bar
		}
	}

	if (currentPage == 7 && enemyVisible) { // New stage: Enemy
		if (isEnemyDead) {
			iShowImage(enemyX, enemyY, 300, 300, enemyDeadFrames[currentEnemyDeadFrame]); // Bigger enemy death animation
		}
		else if (isEnemyAttacking) {
			iShowImage(enemyX, enemyY, 300, 300, enemyAttackFrames[currentEnemyAttackFrame]); // Bigger enemy attack animation
		}
		else {
			iShowImage(enemyX, enemyY, 300, 300, enemyAttackFrames[0]); // Bigger default enemy frame
		}
		drawEnemyHealthBar(); // Draw the enemy's health bar
	}

	for (auto& spell : playerSpells) {
		if (spell.active) {
			if (spell.isSpecial) {
				iShowImage(spell.x, spell.y, 100, 100, specialAttackID); // Special attack visual
			}
			else {
				drawRotatedImage(spell.x, spell.y, 50, 50, spell1ID, spell.angle); // Character uses spell1.png for normal attacks
			}
		}
	}

	for (auto& spell : villainSpells) {
		if (spell.active) {
			if (currentPage == 4) { // 2nd stage
				drawRotatedImage(spell.x, spell.y, 50, 50, spell2ID, spell.angle); // Villain uses spell.png for attacks
			}
			else if (currentPage == 5) { // Boss stage
				drawRotatedImage(spell.x, spell.y, 50, 50, spell3ID, spell.angle); // Boss uses spell3.png
			}
			else if (currentPage == 7) { // New stage: Enemy
				if (spell.isSpecial) {
					iShowImage(spell.x, spell.y, 100, 100, spell6ID); // Draw special attack using spell6.png
				}
				else {
					drawRotatedImage(spell.x, spell.y, 50, 50, spell5ID, spell.angle); // Enemy uses spell5.png for normal attacks
				}
			}
		}
	}

	if (healthKit.active) {
		iShowImage(healthKit.x, healthKit.y, 50, 50, healthKitID);
	}

	iSetColor(255, 255, 255);
	iText(10, 680, killText, GLUT_BITMAP_HELVETICA_18);

	iSetColor(255, 0, 0);
	iFilledRectangle(10, 650, characterHealth * 2, 20);
	iSetColor(255, 255, 255);
	iRectangle(10, 650, 200, 20);
	iText(220, 650, healthText, GLUT_BITMAP_HELVETICA_18);

	// Draw special attack cooldown
	if (!isSpecialAttackReady) {
		iSetColor(255, 255, 255);
		iText(10, 600, "Special Attack Cooldown", GLUT_BITMAP_HELVETICA_18);
	}
}

void updateSpells() {
	// Variables to track whether the sound has been played for each threshold
	static bool played75 = false;
	static bool played50 = false;
	static bool played25 = false;
	static bool played10 = false;

	for (auto& spell : playerSpells) {
		if (spell.active) {
			spell.x += 20;
			spell.angle += 10;
			if (spell.x > 1280) spell.active = false;

			// Check collision with the villain or boss
			if (villainVisible && spell.x + 50 > villainX && spell.x < villainX + 100 && // Villain width = 100
				spell.y + 50 > villainY && spell.y < villainY + 100) { // Villain height = 100
				spell.active = false; // Deactivate the spell

				if (currentPage == 4) { // 2nd stage: Villain
					villainVisible = false; // Villain is defeated
					villainKillCount += 1; // Increment kill count
					sprintf_s(killText, sizeof(killText), "Kills: %d", villainKillCount);

					// Transition to the final stage if 6 villains are killed
					if (villainKillCount == 6) {
						currentPage = 5; // Transition to the final stage
						bossHealth = BOSS_MAX_HEALTH; // Reset boss health
					}
					else {
						iSetTimer(2000, []() { villainVisible = true; }); // Respawn villain
					}
				}
				else if (currentPage == 5) { // Final stage: Boss
					if (spell.isSpecial) {
						bossHealth -= 15; // Special attack deals 15% damage
					}
					else {
						bossHealth -= 5; // Normal attack deals 5% damage
					}

					// Ensure boss health doesn't go below 0
					if (bossHealth < 0) bossHealth = 0;

					// Debug statement to verify collision
					printf("Boss hit! Boss health: %d\n", bossHealth);

					// Check if the boss is defeated
					if (bossHealth == 0) {
						villainVisible = false;
						currentPage = 7; // Transition to the new stage after boss defeat
					}
					else {
						villainVisible = true;
					}
				}
			}

			// Check collision with the enemy in the new stage (currentPage == 7)
			if (currentPage == 7 && enemyVisible && spell.x + 50 > enemyX && spell.x < enemyX + 300 && // Enemy width = 300
				spell.y + 50 > enemyY && spell.y < enemyY + 300) { // Enemy height = 300
				spell.active = false; // Deactivate the spell
				enemyHealth -= 2; // Reduced damage from 10 to 2

				// Play sound effects based on enemy's health percentage
				if (enemyHealth <= 75 && !played75) {
					PlaySound("75percent.wav", NULL, SND_ASYNC); // Play 75% health sound
					played75 = true; // Mark as played
				}
				else if (enemyHealth <= 50 && !played50) {
					PlaySound("50percent.wav", NULL, SND_ASYNC); // Play 50% health sound
					played50 = true; // Mark as played
				}
				else if (enemyHealth <= 25 && !played25) {
					PlaySound("25percent.wav", NULL, SND_ASYNC); // Play 25% health sound
					played25 = true; // Mark as played
				}
				/*else if (enemyHealth <= 10 && !played10) {
				PlaySound("10percent.wav", NULL, SND_ASYNC); // Play 10% health sound
				played10 = true; // Mark as played
				}*/

				if (enemyHealth <= 0) {
					enemyHealth = 0;
					isEnemyDead = true; // Trigger enemy death animation
					currentPage = 9; // Transition to the win screen
				}
			}
		}
	}

	for (auto& spell : villainSpells) {
		if (spell.active) {
			spell.x -= 15;
			spell.angle += 10;
			if (spell.x < 0) spell.active = false;

			// Check collision with the character
			if (spell.x < characterX + 100 && spell.x + 50 > characterX &&
				spell.y < characterY + 100 && spell.y + 50 > characterY) {
				spell.active = false;

				if (spell.isSpecial) { // Special attack kills the character instantly
					characterHealth = 0;
					sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth);
					currentPage = 6; // Game Over screen
					playBackgroundMusic(); // Stop music on Game Over
				}
				else {
					characterHealth -= 10; // Normal attack deals 10% damage
					if (characterHealth < 0) characterHealth = 0;
					sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth);
					healthRegenerationTimer = 0; // Reset the regeneration timer

					if (characterHealth == 0) {
						currentPage = 6; // Game Over screen
						playBackgroundMusic(); // Stop music on Game Over
					}
				}
			}
		}
	}
}

void villainAI() {
	if (villainVisible) {
		// Move villain up and down
		if (villainDirection) {
			villainY += villainSpeed;
			if (villainY > 500) villainDirection = false;
		}
		else {
			villainY -= villainSpeed;
			if (villainY < 100) villainDirection = true;
		}

		// Spawn spells with increased probability
		if (rand() % 20 < 2) { // Increased probability
			villainSpells.push_back({ villainX, villainY, true, 0.0f, false });
			currentVillainFrame = (currentVillainFrame + 1) % 5;
			printf("Villain Spell Spawned!\n"); // Debug
		}
	}
}

void bossAI() {
	if (isCharging) {
		// Charge toward the player
		if (villainX < characterX) villainX += bossSpeed * 2; // Move right
		else if (villainX > characterX) villainX -= bossSpeed * 2; // Move left

		if (villainY < characterY) villainY += bossSpeed * 2; // Move up
		else if (villainY > characterY) villainY -= bossSpeed * 2; // Move down

		// Stop charging if close to the player
		if (abs(villainX - characterX) < 50 && abs(villainY - characterY) < 50) {
			isCharging = false;
			chargeCooldown = CHARGE_COOLDOWN_TIME; // Start cooldown
		}
	}
	else if (isZigzagging) {
		// Zigzag movement
		villainX += bossSpeed * zigzagDirection; // Move left or right
		villainY += bossSpeed * 0.5f; // Move slightly downward

		// Change direction when reaching screen edges
		if (villainX < 0 || villainX > SCREEN_WIDTH - 100) {
			zigzagDirection *= -1; // Reverse direction
		}

		// Randomly stop zigzagging and start another pattern
		if (rand() % 100 < 2) { // 2% chance to stop zigzagging
			isZigzagging = false;
		}
	}
	else {
		// Default movement (up and down)
		if (villainDirection) {
			villainY += bossSpeed;
			if (villainY > 500) villainDirection = false;
		}
		else {
			villainY -= bossSpeed;
			if (villainY < 100) villainDirection = true;
		}

		// Randomly start charging or zigzagging
		if (chargeCooldown <= 0 && rand() % 100 < 2) { // 2% chance to start charging
			isCharging = true;
		}
		if (rand() % 100 < 2) { // 2% chance to start zigzagging
			isZigzagging = true;
			zigzagDirection = (rand() % 2 == 0) ? 1 : -1; // Randomize initial direction
		}
	}

	// Ensure the boss stays within the screen boundaries
	if (villainX < 0) villainX = 0;
	if (villainX > SCREEN_WIDTH - 100) villainX = SCREEN_WIDTH - 100; // Adjust for boss width
	if (villainY < 0) villainY = 0;
	if (villainY > SCREEN_HEIGHT - 150) villainY = SCREEN_HEIGHT - 150; // Adjust for boss height

	// Health regeneration
	if (bossRegenerationTimer > 0) {
		bossRegenerationTimer--;
	}
	else {
		if (bossHealth < BOSS_MAX_HEALTH) {
			bossHealth += 10; // Regenerate 10 health
			if (bossHealth > BOSS_MAX_HEALTH) bossHealth = BOSS_MAX_HEALTH;
		}
		bossRegenerationTimer = BOSS_REGENERATION_INTERVAL; // Reset the timer
	}

	// Update cooldowns
	if (chargeCooldown > 0) chargeCooldown--;

	// Spawn spells
	if (villainVisible && rand() % 30 < 2) {
		villainSpells.push_back({ villainX, villainY, true, 0.0f, false }); // Boss uses spell3.png
		currentBossFrame = (currentBossFrame + 1) % 7;
	}
}

void enemyAI() {
	if (currentPage == 7) { // New stage: Enemy
		if (isEnemyDead) {
			enemyDeadTimer++;
			if (enemyDeadTimer >= 30) { // Death animation lasts for 30 frames
				enemyVisible = false; // Enemy disappears after death
			}
			else {
				currentEnemyDeadFrame = (currentEnemyDeadFrame + 1) % 3; // Cycle through death frames
			}
		}
		else if (isEnemyAttacking) {
			enemyAttackTimer++;
			if (enemyAttackTimer >= 20) { // Attack animation lasts for 20 frames
				isEnemyAttacking = false;
			}
			else {
				currentEnemyAttackFrame = (currentEnemyAttackFrame + 1) % 5; // Cycle through attack frames
			}
		}
		else {
			// Randomly decide whether to move closer to the player (30% chance)
			if (rand() % 100 < 30) {
				if (enemyX < characterX) enemyX += enemySpeed; // Move right
				else if (enemyX > characterX) enemyX -= enemySpeed; // Move left

				if (enemyY < characterY) enemyY += enemySpeed; // Move up
				else if (enemyY > characterY) enemyY -= enemySpeed; // Move down
			}

			// Ensure the enemy stays within the screen boundaries
			if (enemyX < 0) enemyX = 0;
			if (enemyX > SCREEN_WIDTH - 300) enemyX = SCREEN_WIDTH - 300; // Adjust for enemy width
			if (enemyY < 0) enemyY = 0;
			if (enemyY > SCREEN_HEIGHT - 300) enemyY = SCREEN_HEIGHT - 300; // Adjust for enemy height

			// Health regeneration
			if (bossRegenerationTimer > 0) {
				bossRegenerationTimer--;
			}
			else {
				if (enemyHealth < ENEMY_MAX_HEALTH) {
					enemyHealth += 10; // Regenerate 10 health
					if (enemyHealth > ENEMY_MAX_HEALTH) enemyHealth = ENEMY_MAX_HEALTH;
				}
				bossRegenerationTimer = BOSS_REGENERATION_INTERVAL; // Reset the timer
			}

			// Update cooldowns
			if (chargeCooldown > 0) chargeCooldown--;

			// Spawn spells
			if (rand() % 30 < 2) { // Spawn spells with a 2% chance per frame
				villainSpells.push_back({ enemyX, enemyY + 100, true, 0.0f, false }); // Enemy uses spell5.png
				isEnemyAttacking = true; // Start attack animation
				enemyAttackTimer = 0; // Reset attack animation timer
			}

			// Update special attack cooldown
			if (enemySpecialAttackCooldown > 0) {
				enemySpecialAttackCooldown--;
			}
			else {
				// Spawn special attack
				villainSpells.push_back({ enemyX, enemyY + 150, true, 0.0f, true }); // Use spell6.png for special attack
				enemySpecialAttackCooldown = ENEMY_SPECIAL_ATTACK_INTERVAL; // Reset cooldown
			}
		}
	}
}

void spawnHealthKit() {
	int spawnRate = (currentPage == 5 || currentPage == 7) ? 50 : 600; // Health kits spawn twice as often in final stage or new stage
	if (!healthKit.active && rand() % spawnRate == 0) {
		healthKit.x = 1280;
		healthKit.y = rand() % 600 + 50;
		healthKit.active = true;
	}
}

void updateHealthKit() {
	if (healthKit.active) {
		healthKit.x -= 5;

		if (healthKit.x < characterX + 100 && healthKit.x + 50 > characterX &&
			healthKit.y < characterY + 100 && healthKit.y + 50 > characterY) {
			healthKit.active = false;
			characterHealth += 10;
			if (characterHealth > 100) characterHealth = 100;
			sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth);
			healthRegenerationTimer = 0; // Reset the regeneration timer
		}

		if (healthKit.x < -50) {
			healthKit.active = false;
		}
	}
}

void drawLogo() {
	iSetColor(0, 0, 0);
	iFilledRectangle(logoX, logoY, logoWidth, logoHeight);
	iSetColor(255, 255, 255);
	iFilledRectangle(logoX + 15, logoY + 15, logoWidth - 30, logoHeight - 30);
}

void moveLogo() {
	logoX += dx;
	logoY += dy;

	if (abs(logoX - (SCREEN_WIDTH / 2 - logoWidth / 2)) < 5 &&
		abs(logoY - (SCREEN_HEIGHT / 2 - logoHeight / 2)) < 5) {
		logoX = SCREEN_WIDTH / 2 - logoWidth / 2;
		logoY = SCREEN_HEIGHT / 2 - logoHeight / 2;
		dx = 0;
		dy = 0;
		currentPage = 2; // Transition to the second image
	}

	if (logoX + logoWidth > SCREEN_WIDTH || logoX < 0) {
		dx *= -1;
	}
	if (logoY + logoHeight > SCREEN_HEIGHT || logoY < 0) {
		dy *= -1;
	}
}

void iDraw() {
	iClear();
	if (currentPage == -1) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, startingScreenID);
		iSetColor(255, 255, 255);
		iText(500, 100, "Press ENTER to continue", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (currentPage == 0) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, mainMenuBackgroundID);
		drawMenu();
	}
	else if (currentPage == 1) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, playGameScreenID);
		iSetColor(255, 255, 255);
		iText(500, 50, "Press ENTER to reveal the map", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (currentPage == 2) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, secondImageID);
		iSetColor(255, 255, 255);
		iText(500, 50, "Press ENTER to begin your trial", GLUT_BITMAP_TIMES_ROMAN_24);
	}
	else if (currentPage == 3) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backgroundImage);
		iSetColor(0, 0, 0);
		iRectangle(SCREEN_WIDTH / 2 - 25, SCREEN_HEIGHT / 2 - 25, 50, 50);
		drawLogo();
	}
	else if (currentPage == 4 || currentPage == 5 || currentPage == 7) {
		drawLevel();
	}
	else if (currentPage == 6) { // Game Over screen
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, overScreenID); // Show over.jpg
		iSetColor(255, 255, 255); // White color for text
		// Display "Press ESC to return to the main menu" in the bottom-right corner
		iText(SCREEN_WIDTH - 350, 30, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
	}
	else if (currentPage == 8) {
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, aboutUsBackgroundID);
	}
	else if (currentPage == 9) { // Win screen
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, winScreenID); // Show win.jpg
		iSetColor(255, 255, 255); // White color for text
		// Display "Press ESC to return to the main menu" in the bottom-right corner
		iText(SCREEN_WIDTH - 350, 30, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
	}
	else if (currentPage == 10) { // Instruction screen
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, instructionScreenID); // Show instruction1.jpg
		iSetColor(255, 255, 255); // White color for text
		// Display "Press ESC to return to the main menu" in the bottom-right corner
		iText(SCREEN_WIDTH - 350, 30, "Press ESC to return to the main menu", GLUT_BITMAP_HELVETICA_18);
	}
}

void iKeyboard(unsigned char key) {
	if (key == 27) { // ESC key
		if (currentPage == 6 || currentPage == 9 || currentPage == 10) { // Game Over, Win, or Instruction screen
			// Reset all game state variables
			characterHealth = 100; // Reset character health
			sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth); // Update health text
			villainKillCount = 0; // Reset kill count
			sprintf_s(killText, sizeof(killText), "Kills: %d", villainKillCount); // Update kill text
			bossHealth = BOSS_MAX_HEALTH; // Reset boss health
			enemyHealth = ENEMY_MAX_HEALTH; // Reset enemy health
			isEnemyDead = false; // Reset enemy death state
			isEnemyAttacking = false; // Reset enemy attack state
			enemyVisible = true; // Make enemy visible again
			playerSpells.clear(); // Clear player spells
			villainSpells.clear(); // Clear villain spells
			healthKit.active = false; // Reset health kit
			isSpecialAttackReady = true; // Reset special attack cooldown
			specialAttackCooldown = 0; // Reset special attack timer
			currentPage = 0; // Return to the main menu
			playBackgroundMusic(); // Restart background music if sound is on
		}
		else if (currentPage != 0) {
			currentPage = 0; // Return to the main menu
		}
		else {
			exit(0); // Exit the game
		}
	}
	else if (key == 13) { // ENTER key
		if (currentPage == -1) {
			currentPage = 0;
		}
		else if (currentPage == 1) {
			currentPage = 3; // Transition to the new level
		}
		else if (currentPage == 2) {
			currentPage = 4; // Transition to the fighting stage
			characterHealth = 100;
			hitsTaken = 0; // Reset the hits counter
			sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth);
			villainVisible = true; // Ensure villain is visible
		}
	}
	else if (key == ' ') {
		if (currentPage == 4 || currentPage == 5 || currentPage == 7) {
			playerSpells.push_back({ characterX + 50, characterY + 25, true, 0.0f, false }); // Normal attack uses spell1.png
			isAttacking = true; // Start attack animation
			attackAnimationTimer = 0; // Reset attack animation timer
		}
	}
	else if (key == 's' || key == 'S') {
		if ((currentPage == 5 || currentPage == 7) && isSpecialAttackReady) { // Only in boss stage or new stage
			playerSpells.push_back({ characterX + 50, characterY + 25, true, 0.0f, true });
			isSpecialAttackReady = false;
			specialAttackCooldown = SPECIAL_ATTACK_COOLDOWN_TIME;
			// Removed the PlaySound call for special attack
		}
	}
}

void iSpecialKeyboard(unsigned char key) {
	if (currentPage == 3) {
		if (key == GLUT_KEY_RIGHT) {
			dx = boxSpeed; // Increased box speed
			dy = 0;
		}
		else if (key == GLUT_KEY_LEFT) {
			dx = -boxSpeed; // Increased box speed
			dy = 0;
		}
		else if (key == GLUT_KEY_UP) {
			dx = 0;
			dy = boxSpeed; // Increased box speed
		}
		else if (key == GLUT_KEY_DOWN) {
			dx = 0;
			dy = -boxSpeed; // Increased box speed
		}
	}
	else if (currentPage == 4 || currentPage == 5 || currentPage == 7) {
		if (key == GLUT_KEY_RIGHT) characterX += characterSpeed;
		else if (key == GLUT_KEY_LEFT) characterX -= characterSpeed;
		else if (key == GLUT_KEY_UP) {
			characterY += characterSpeed;
			if (characterY > 500) characterY = 500; // Restrict vertical movement
		}
		else if (key == GLUT_KEY_DOWN) {
			characterY -= characterSpeed;
			if (characterY < 100) characterY = 100; // Restrict vertical movement
		}
	}
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (currentPage == 0) {
			for (int i = 0; i < 5; i++) {
				if (menu[i].isClicked(mx, my)) {
					if (i == 0) {
						currentPage = 1;
					}
					else if (i == 1) { // Instructions option
						currentPage = 10; // Go to instruction screen
					}
					else if (i == 2) {
						currentPage = 8;
					}
					else if (i == 3) {
						soundOn = !soundOn;
						playBackgroundMusic();
					}
					else if (i == 4) {
						exit(0);
					}
				}
			}
		}
	}
}

void iMouseMove(int mx, int my) {}

void iPassiveMouseMove(int mx, int my) {}

int main() {
	srand(static_cast<unsigned>(time(0)));
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Game with Animated Hero & Villain");

	startingScreenID = iLoadImage("starting1.jpg");
	mainMenuBackgroundID = iLoadImage("background.jpg");
	aboutUsBackgroundID = iLoadImage("about_us_2.jpg");
	playGameScreenID = iLoadImage("first.jpg");
	secondImageID = iLoadImage("second.jpg");
	thirdImageID = iLoadImage("background2.jpg");
	levelBackgroundID = iLoadImage("Background1.jpg");
	backgroundImage = iLoadImage("door.jpg");
	newStageBackgroundID = iLoadImage("backgroundfinal.jpg"); // Load the new stage background
	winScreenID = iLoadImage("win.jpg"); // Load the win screen image
	overScreenID = iLoadImage("over.jpg"); // Load the game over screen image
	instructionScreenID = iLoadImage("instruction1.jpg"); // Load the instruction screen image

	for (int i = 0; i < 10; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "Untitled-%d.png", i + 1);
		characterFrames[i] = iLoadImage(filename);
	}

	for (int i = 0; i < 5; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "villain%d.png", i + 1);
		villainFrames[i] = iLoadImage(filename);
	}

	for (int i = 0; i < 7; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "boss%d.png", i + 1);
		bossFrames[i] = iLoadImage(filename);
	}

	// Load enemy attack frames
	for (int i = 0; i < 5; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "attack_%d.png", i + 1);
		enemyAttackFrames[i] = iLoadImage(filename);
	}

	// Load enemy death frames
	for (int i = 0; i < 3; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "dead_%d.png", i + 1);
		enemyDeadFrames[i] = iLoadImage(filename);
	}

	spell1ID = iLoadImage("spell1.png"); // Character uses spell1.png for normal attacks
	spell2ID = iLoadImage("spell.png");  // Villain uses spell.png for attacks
	spell3ID = iLoadImage("spell3.png"); // Boss uses spell3.png
	spell5ID = iLoadImage("spell5.png"); // Enemy uses spell5.png
	spell6ID = iLoadImage("spell6.png"); // Load the special attack image
	healthKitID = iLoadImage("healthKit.png");
	specialAttackID = iLoadImage("pngfind.com-skyrim-logo-png-413340 - Copy.png"); // Load special attack image

	// Load flying and attack animations
	flyID = iLoadImage("fly.png");
	for (int i = 0; i < 3; i++) {
		char filename[20];
		sprintf_s(filename, sizeof(filename), "attack%d.png", i + 1);
		attackFrames[i] = iLoadImage(filename);
	}

	iSetTimer(150, []() {
		currentCharacterFrame = (currentCharacterFrame + 1) % 4; // Walking animation
		if (isAttacking) {
			attackAnimationTimer++;
			if (attackAnimationTimer >= 10) { // Attack animation lasts for 10 frames
				isAttacking = false;
			}
			else {
				currentAttackFrame = (currentAttackFrame + 1) % 3; // Cycle through attack frames
			}
		}
	});

	iSetTimer(150, []() {
		if (currentPage == 3) {
			moveLogo();
		}
		else if (currentPage == 4 || currentPage == 5 || currentPage == 7) {
			updateSpells();
			if (currentPage == 4) villainAI();
			else if (currentPage == 5) bossAI();
			else if (currentPage == 7) enemyAI();
			spawnHealthKit();
			updateHealthKit();
			backgroundX -= backgroundSpeed;
			if (backgroundX <= -1280) backgroundX = 0;

			// Update special attack cooldown
			if (!isSpecialAttackReady) {
				specialAttackCooldown--;
				if (specialAttackCooldown <= 0) {
					isSpecialAttackReady = true;
				}
			}

			// Health regeneration in the third stage (currentPage == 5)
			if (currentPage == 5) {
				healthRegenerationTimer++;
				if (healthRegenerationTimer >= HEALTH_REGENERATION_INTERVAL) {
					characterHealth += 10; // Regenerate 10% health
					if (characterHealth > 100) characterHealth = 100; // Cap health at 100%
					sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth); // Update health text
					healthRegenerationTimer = 0; // Reset the timer
				}
			}

			// Enemy health regeneration in the new stage (currentPage == 7)
			if (currentPage == 7) {
				enemyRegenerationTimer++;
				if (enemyRegenerationTimer >= ENEMY_REGENERATION_INTERVAL) {
					enemyHealth += 10; // Regenerate 10% health
					if (enemyHealth > ENEMY_MAX_HEALTH) enemyHealth = ENEMY_MAX_HEALTH; // Cap health at max
					enemyRegenerationTimer = 0; // Reset the timer
				}
			}
		}
	});

	iSetTimer(50, []() {
		if (currentPage == 3) {
			moveLogo();
		}
		else if (currentPage == 4 || currentPage == 5 || currentPage == 7) {
			updateSpells();
			if (currentPage == 4) villainAI();
			else if (currentPage == 5) bossAI();
			else if (currentPage == 7) enemyAI();
			spawnHealthKit();
			updateHealthKit();
			backgroundX -= backgroundSpeed;
			if (backgroundX <= -1280) backgroundX = 0;

			// Update special attack cooldown
			if (!isSpecialAttackReady) {
				specialAttackCooldown--;
				if (specialAttackCooldown <= 0) {
					isSpecialAttackReady = true;
				}
			}

			// Health regeneration in the third stage (currentPage == 5)
			if (currentPage == 5) {
				healthRegenerationTimer++;
				if (healthRegenerationTimer >= HEALTH_REGENERATION_INTERVAL) {
					characterHealth += 10; // Regenerate 10% health
					if (characterHealth > 100) characterHealth = 100; // Cap health at 100%
					sprintf_s(healthText, sizeof(healthText), "Health: %d%%", characterHealth); // Update health text
					healthRegenerationTimer = 0; // Reset the timer
				}
			}
		}
	});

	playBackgroundMusic();
	iStart();
}