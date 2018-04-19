#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <iostream>
#include "List.h"
#include "Random.h"
#include "HashTable.h"
#include "Hero.h"
#include "BST.h"
#include "Particle System.h"
#include <fstream>
#include <vector>
#include <sstream>  


void launchMenu(sf::RenderWindow &window, sf::Vector2i &screenDimensions);
void launchChampionMenu(Hero Champion, List<Equipment>* Store_Inventory, sf::RenderWindow &window, sf::Vector2i &screenDimensions, sf::Font &font);

template<class T>
std::string convertToString(const T& data) // Converts any data so we can output it into SFML
{
	std::ostringstream stream;
	stream << data;
	return stream.str();
}

//======================================================= Local Function to main() Driver

// Must Run at Start
void getData(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* champHealth, BST<int>* equipHealth);

// Run After Sort
void reHashChampions(List<Hero> Champions, HashTable* Table);
void reHashInventory(List<Equipment> Inventory, HashTable* Table);

// Menus
char mainMenu();
char championHallMenu();
char searchMenu();
char attributeMenu();
char equipmentRoomMenu();

//Switch case Functions
void addChampion(List<Hero>* Champions, HashTable* Table, BST<int>* tree);
void addEquipment(List<Equipment>* Inventory, HashTable* Table, BST<int>* tree);
void print_to_file(List<Hero> Champions, List<Equipment> Inventory);
void search(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree, bool remove, char typeSwitch);
void equipChampion(List<Hero>* Champions, List<Equipment> Inventory);
void dequipChampion(List<Hero>* Champions);
void equipmentRoom(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree);
void championHall(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree);

//Other functions
void battle(List<Hero>* hero);


int main()
{
	sf::Vector2i screenDimensions(1920, 1080);
	sf::RenderWindow window(sf::VideoMode(screenDimensions.x, screenDimensions.y), "League Project", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	//sf::Texture paddleLarge;
	sf::Texture leagueLogo;
	sf::Texture leagueLaunch;
	sf::Texture leagueLaunchGlow;
	sf::Texture samuraiOwl;

	ParticleSystem particles(10000);

	// Font
	sf::Font font;

	if (!font.loadFromFile("LSANSDI.ttf"))
	{
		// handle error
	}

	//************************************** User Input ********************************************************
	sf::String sentence; // Used for user input

	sf::Text userInput(sentence, font, 30); // Construct the format for the user's input sentence
	userInput.setColor(sf::Color(32, 178, 206));
	userInput.setStyle(sf::Text::Bold | sf::Text::Underlined);
	userInput.setPosition(344, 322);
	//************************************End of User Input*****************************************************

	sf::Text credits("Creators\nNathan Foster\nDalton Rick\nKush Patel\nDesmond Lee", font, 60);
	credits.setPosition(screenDimensions.x / 2.0f - credits.getGlobalBounds().width / 2.0f, 400);
	credits.setColor(sf::Color(32, 178, 206));
	credits.setStyle(sf::Text::Bold | sf::Text::Underlined);

	if (!leagueLogo.loadFromFile("League Logo.png"))
		window.close();

	if (!leagueLaunch.loadFromFile("League Launch.png"))
		window.close();

	if (!leagueLaunchGlow.loadFromFile("League Launch Glow.png")) // sf::Sprite sprite_LeagueLaunchGlow(leagueLaunchGlow);
		window.close();

	if (!samuraiOwl.loadFromFile("Samurai Owl.png"))
		window.close();

	sf::Sprite sprite_LeagueLogo(leagueLogo);
	sprite_LeagueLogo.setPosition(screenDimensions.x / 2.0f - sprite_LeagueLogo.getGlobalBounds().width / 2.0f, 0);

	sf::Sprite sprite_LeagueLaunch(leagueLaunch);
	sprite_LeagueLaunch.setPosition(100, 50);

	sf::Sprite sprite_SamuraiOwl(samuraiOwl);
	sprite_SamuraiOwl.setPosition(sprite_LeagueLaunch.getGlobalBounds().width / 2.0f, 500);

	// Audio
	sf::SoundBuffer mainMenuBuffer;
	if (!mainMenuBuffer.loadFromFile("MainMenuMusic.ogg"))
		window.close();

	sf::Sound mainMenuMusic;
	mainMenuMusic.setBuffer(mainMenuBuffer);
	mainMenuMusic.play();

	sf::SoundBuffer launchBuffer;
	if (!launchBuffer.loadFromFile("LaunchSound.ogg"))
		window.close();

	sf::Sound launchSound;
	launchSound.setBuffer(launchBuffer);

	// create a clock to track the elapsed time
	sf::Clock clock;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::TextEntered: // Code for user input
				if (event.text.unicode >= 32 && event.text.unicode <= 126)
					sentence += (char)event.text.unicode;
				else if (event.text.unicode == 8 && sentence.getSize() > 0)
					sentence.erase(sentence.getSize() - 1, 1);

				userInput.setString(sentence);
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			case sf::Keyboard::Escape:
				window.close();
				break;

			case::sf::Event::MouseMoved:
				// Check for sprites hovered
				if (sprite_LeagueLaunch.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					sprite_LeagueLaunch.setTexture(leagueLaunchGlow);

				else
					sprite_LeagueLaunch.setTexture(leagueLaunch);

				break;
			case sf::Event::MouseButtonPressed:
				if (sprite_LeagueLaunch.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					window.clear();
					launchSound.play();
					launchMenu(window, screenDimensions);
				}
				break;
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}

		// make the particle system emitter follow the mouse
		sf::Vector2i mouse = sf::Mouse::getPosition(window);
		particles.setEmitter(window.mapPixelToCoords(mouse));

		// update it
		sf::Time elapsed = clock.restart();
		particles.update(elapsed);

		window.clear();

		//window.draw(sprite_Paddle);

		window.draw(sprite_LeagueLogo);

		window.draw(sprite_LeagueLaunch);

		window.draw(sprite_SamuraiOwl);

		window.draw(credits);

		window.draw(userInput);

		window.draw(particles);

		window.display();


		// ------------------------------------------------->THIS IS WHERE WE NEED TO IMPLEMENT IT INTO SFML <----------------------------------------------------------------------------

		//Main Menu
		/*
		while ((choice = mainMenu()) && choice != 'Q') {
		switch (choice) {
		case 'C':
		championHall(&Champions, &Inventory, &Table);
		break;
		case 'E':
		equipmentRoom(&Champions, &Inventory, &Table);
		break;
		default:
		cout << endl << choice << " is not a valid choice!\n";
		break;
		}
		}
		//======================================================= Save any New Champions or data to file before exit
		print_to_file(Champions, Inventory);
		*/
	}
}

//=====================================================================================================================================================================SFML Functions
void launchMenu(sf::RenderWindow &window, sf::Vector2i &screenDimensions)
{
	List<Hero> Champions; List<Equipment> Inventory; HashTable Table;  char choice;
	BST<int> champHealth; BST<int> equipHealth;
	/* JUST IN CASE ANYONE DECIDES TO BST THE WHOLE DAMN ATTRIBUTE LIST
	BST<int> manaTree; BST<int> aRangeTree; BST<int> aDamageTree; BST<double> aSpeedTree; BST<double> armorTree; BST<double> magResTree; BST<int> moveSpeedTree;
	*/
	getData(&Champions, &Inventory, &Table, &champHealth, &equipHealth);

	// Textures
	sf::Texture leaguePlay;
	if (!leaguePlay.loadFromFile("League Play.png"))
		window.close();

	sf::Texture leaguePlayGlow;
	if (!leaguePlayGlow.loadFromFile("League Play Glow.png"))
		window.close();

	sf::Texture textBox;
	if (!textBox.loadFromFile("Text Box.png"))
		window.close();

	sf::Texture Exit;
	if (!Exit.loadFromFile("Exit.png"))
		window.close();
	Exit.setSmooth(true);
	//Sprites

	sf::Sprite sprite_LeaguePlay(leaguePlay);
	sprite_LeaguePlay.setPosition(50, 50);

	sf::Sprite sprite_LeaguePlayGlow(leaguePlayGlow);
	sprite_LeaguePlayGlow.setPosition(50, 50);

	sf::Sprite sprite_textBox(textBox);
	sprite_textBox.setPosition(screenDimensions.x / 2.0f - sprite_textBox.getGlobalBounds().width / 2.0f, sprite_LeaguePlay.getPosition().y);

	sf::Sprite sprite_Exit(Exit);
	sprite_Exit.setPosition(screenDimensions.x - 250, window.getPosition().y);

	// Audio
	sf::SoundBuffer launchBuffer;
	if (!launchBuffer.loadFromFile("LaunchSound.ogg"))
		window.close();

	sf::Sound launchSound;
	launchSound.setBuffer(launchBuffer);

	sf::SoundBuffer playBuffer;
	if (!playBuffer.loadFromFile("PlaySound.ogg"))
		window.close();

	sf::Sound playSound;
	playSound.setBuffer(playBuffer);

	// Font
	sf::Font font;

	if (!font.loadFromFile("LSANSDI.ttf"))
	{
		// handle error
	}

	//************************************** String vector to store data ***************************************

	std::vector<sf::Text> vTexts; // Vector to store data that will be converted in SFML strings
	
	Champions.begin();

	float y = 200; // Y coordinate to be augmented.

	while (!Champions.off_end())
	{
		sf::Text heroName(convertToString(Champions.current().getName()), font, 40); // convertToString function call accepts any data and converts it into a cString of which SMFL utilizes.
		heroName.setPosition(screenDimensions.x / 2.0f - heroName.getGlobalBounds().width / 2.0f, y);
		heroName.setColor(sf::Color(32, 178, 206));
		heroName.setStyle(sf::Text::Style::Underlined);
		vTexts.push_back(heroName);
		Champions.scroll();
		y += 50;
	}
	//***************************************End of String Vector************************************************

	const int SCROLL_SPEED = 20; // 20 pixels per second

	sf::Vector2f Exit_defaultPos = sprite_Exit.getPosition();

	string nameToBeHased;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			case sf::Event::MouseMoved:
				if (sprite_LeaguePlay.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					sprite_LeaguePlay.setTexture(leaguePlayGlow);

				else // reset back to orginal
					sprite_LeaguePlay.setTexture(leaguePlay);


				if (sprite_Exit.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					sprite_Exit.setPosition(Exit_defaultPos.x - 50.0f, Exit_defaultPos.y);

					sprite_Exit.setScale(1.25, 1.25);

					sprite_Exit.setColor(sf::Color::Red);
				}
				else // reset back to orginal
				{
					sprite_Exit.setPosition(Exit_defaultPos.x, Exit_defaultPos.y);

					sprite_Exit.setScale(1, 1);

					sprite_Exit.setColor(sf::Color::White);
				}


				// Check for texts hovered
				for (int i = 0; i < vTexts.size(); ++i)
				{
					if (vTexts.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
						vTexts.at(i).setColor(sf::Color::White);
					else
						vTexts.at(i).setColor(sf::Color(32, 178, 206));
				}

				break;
			case sf::Event::MouseButtonPressed:
				if (sprite_LeaguePlay.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					playSound.play();
				if (sprite_Exit.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					window.close();

				for (int i = 0; i < vTexts.size(); ++i)
				{
					if (vTexts.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					{
						playSound.play();
						// Convert name back to std::string
						nameToBeHased = vTexts.at(i).getString();
						// Search name using hash table
						Table.findName(nameToBeHased);
						// Scroll to the index returned by the hash function
						Champions.scrollToIndex(Table.findName(nameToBeHased));
						// Pass the Champion scrolled to
						launchChampionMenu(Champions.current(), &Inventory, window, screenDimensions, font);

						window.clear();
					}
				}
				break;

			case sf::Event::MouseWheelMoved:
				for (int i = 0; i < vTexts.size(); i++) // Move all the texts in the vector
				{
					// If mouse wheel is scrolled down move down
					if (event.mouseWheel.delta == -1)
						vTexts.at(i).move(0.f, SCROLL_SPEED);
					// Else move down
					else
						vTexts.at(i).move(0.f, -SCROLL_SPEED);
				}
				break;
			case sf::Event::Closed:
				window.close();
				print_to_file(Champions, Inventory);
				break;
			default:
				break;
			}

			window.clear();
			window.draw(sprite_LeaguePlay);
			window.draw(sprite_textBox);
			window.draw(sprite_Exit);

			for (const auto& textHeroNames : vTexts) // Display vector of strings
			{
				// Don't draw texts that are outside the text box
				if (textHeroNames.getPosition().y > sprite_textBox.getPosition().y)
					window.draw(textHeroNames);
			}

			window.display();
		}

	}
}
void launchChampionMenu(Hero Champion, List<Equipment>* Store_Inventory, sf::RenderWindow &window, sf::Vector2i &screenDimensions, sf::Font &font)
{
	HashTable Table;
	// Textures
	sf::Texture homeButtonArrow;
	if (!homeButtonArrow.loadFromFile("Home Button Arrow.png"))
		window.close();
	homeButtonArrow.setSmooth(true);

	sf::Texture homeButton;
	if (!homeButton.loadFromFile("Home Button.png"))
		window.close();
	homeButton.setSmooth(true);

	sf::Texture leagueLaunchGlow;
	if (!leagueLaunchGlow.loadFromFile("League Launch Glow.png")) // sf::Sprite sprite_LeagueLaunchGlow(leagueLaunchGlow);
		window.close();

	sf::Texture itemShop;
	if (Champion.getName() == "Darius")
		if (!itemShop.loadFromFile("shop_Darius.png"))
			window.close();

	if (Champion.getName() == "Diana")
		if (!itemShop.loadFromFile("shop_Diana.png"))
			window.close();

	if (Champion.getName() == "Fiora")
		if (!itemShop.loadFromFile("shop_Fiora.png"))
			window.close();

	if (Champion.getName() == "Gangplank")
		if (!itemShop.loadFromFile("shop_GangPlank.png"))
			window.close();

	if (Champion.getName() == "Garen")
		if (!itemShop.loadFromFile("shop_Garen.png"))
			window.close();

	if (Champion.getName() == "Hecarim")
		if (!itemShop.loadFromFile("shop_Hecarim.png"))
			window.close();

	if (Champion.getName() == "LeBlanc")
		if (!itemShop.loadFromFile("shop_LeBlanc.png"))
			window.close();

	if (Champion.getName() == "Lissandra")
		if (!itemShop.loadFromFile("shop_Lissandra.png"))
			window.close();

	if (Champion.getName() == "Lulu")
		if (!itemShop.loadFromFile("shop_Lulu.png"))
			window.close();

	if (Champion.getName() == "Lux")
		if (!itemShop.loadFromFile("shop_Lux.png"))
			window.close();

	if (Champion.getName() == "Malphite")
		if (!itemShop.loadFromFile("shop_Malphite.png"))
			window.close();

	if (Champion.getName() == "Mordekaiser")
		if (!itemShop.loadFromFile("shop_Mordekaiser.png"))
			window.close();

	if (Champion.getName() == "Nidalee")
		if (!itemShop.loadFromFile("shop_Nidalee.png"))
			window.close();

	if (Champion.getName() == "Olaf")
		if (!itemShop.loadFromFile("shop_Olaf.png"))
			window.close();

	if (Champion.getName() == "Poppy")
		if (!itemShop.loadFromFile("shop_Poppy.png"))
			window.close();

	if (Champion.getName() == "Rammus")
		if (!itemShop.loadFromFile("shop_Rammus.png"))
			window.close();

	if (Champion.getName() == "Riven")
		if (!itemShop.loadFromFile("shop_Riven.png"))
			window.close();

	if (Champion.getName() == "Sion")
		if (!itemShop.loadFromFile("shop_Sion.png"))
			window.close();

	if (Champion.getName() == "Sona")
		if (!itemShop.loadFromFile("shop_Sona.png"))
			window.close();

	if (Champion.getName() == "Syndra")
		if (!itemShop.loadFromFile("shop_Syndra.png"))
			window.close();

	if (Champion.getName() == "Teemo")
		if (!itemShop.loadFromFile("shop_Teemo.png"))
			window.close();

	if (Champion.getName() == "Twitch")
		if (!itemShop.loadFromFile("shop_Twitch.png"))
			window.close();

	if (Champion.getName() == "Vayne")
		if (!itemShop.loadFromFile("shop_Vayne.png"))
			window.close();

	if (Champion.getName() == "Veigar")
		if (!itemShop.loadFromFile("shop_Veigar.png"))
			window.close();

	if (Champion.getName() == "Yorick")
		if (!itemShop.loadFromFile("shop_Yorick.png"))
			window.close();

	if (Champion.getName() == "Zed")
		if (!itemShop.loadFromFile("shop_Zed.png"))
			window.close();
	
	itemShop.setSmooth(true);

	//*********************************************************************************************Item textures
	sf::Texture inventory;
	if (!inventory.loadFromFile("Inventory.png"))
		window.close();

	sf::Texture bansheesVeil;
	if (!bansheesVeil.loadFromFile("item_Banshee's_Veil.png"))
		window.close();

	sf::Texture bladeOfTheRuinedKing;
	if (!bladeOfTheRuinedKing.loadFromFile("item_Blade_of_the_Ruined_King.png"))
		window.close();

	sf::Texture deadMansPlate;
	if (!deadMansPlate.loadFromFile("item_Dead_Man's_Plate.png"))
		window.close();

	sf::Texture frozenHeart;
	if (!frozenHeart.loadFromFile("item_Frozen_Heart.png"))
		window.close();

	sf::Texture frozenMallet;
	if (!frozenMallet.loadFromFile("item_Frozen_Mallet.png"))
		window.close();

	sf::Texture infinityEdge;
	if (!infinityEdge.loadFromFile("item_Infinity_Edge.png"))
		window.close();

	sf::Texture lichBane;
	if (!lichBane.loadFromFile("item_Lich_Bane.png"))
		window.close();

	sf::Texture ludensEcho;
	if (!ludensEcho.loadFromFile("item_Luden's_Echo.png"))
		window.close();

	sf::Texture mawOfMalmortius;
	if (!mawOfMalmortius.loadFromFile("item_Maw_of_Malmortius.png"))
		window.close();

	sf::Texture mercurysThreads;
	if (!mercurysThreads.loadFromFile("item_Mercury's_Treads.png"))
		window.close();

	sf::Texture nashorsTooth;
	if (!nashorsTooth.loadFromFile("item_Nashor's_Tooth.png"))
		window.close();

	sf::Texture ravenousHydra;
	if (!ravenousHydra.loadFromFile("item_Ravenous_Hyrda.png"))
		window.close();

	sf::Texture rodOfAges;
	if (!rodOfAges.loadFromFile("item_Rod_of_Ages.png"))
		window.close();

	sf::Texture steraksGage;
	if (!steraksGage.loadFromFile("item_Sterak's Gage.png"))
		window.close();

	sf::Texture theBloodthrister;
	if (!theBloodthrister.loadFromFile("item_The_Bloodthirster.png"))
		window.close();

	sf::Texture thornmail;
	if (!thornmail.loadFromFile("item_Thornmail.png"))
		window.close();

	sf::Texture titanicHydra;
	if (!titanicHydra.loadFromFile("item_Titanic_Hydra.png"))
		window.close();

	sf::Texture trinityForce;
	if (!trinityForce.loadFromFile("item_Trinity_Force.png"))
		window.close();

	// Sprites
	enum sprite_Names {
		Home_Button, Home_Button_Arrow, Item_Shop, Inventory, // Used to identify sprite names. Must be in correct order.
		Banshees_Veil, Blade_Of_The_Ruined_King, Dead_Mans_Plate, Frozen_Heart, Frozen_Mallet
	};

	std::vector<sf::Sprite> v_Sprites;
	std::vector<sf::Sprite> v_Sprites_Items;

	sf::Sprite sprite_HomeButton(homeButton);
	sprite_HomeButton.setPosition(screenDimensions.x - 250, 35);
	sprite_HomeButton.setColor(sf::Color(32, 178, 206));
	v_Sprites.push_back(sprite_HomeButton);

	sf::Sprite sprite_HomeButtonArrow(homeButtonArrow);
	sprite_HomeButtonArrow.setPosition(screenDimensions.x - 250, 35);
	sprite_HomeButtonArrow.setColor(sf::Color::Black);
	v_Sprites.push_back(sprite_HomeButtonArrow);

	sf::Sprite sprite_ItemShop(itemShop);
	sprite_ItemShop.setScale(0.75, 0.75);
	sprite_ItemShop.setPosition(screenDimensions.x / 2.0f - sprite_ItemShop.getGlobalBounds().width / 2.0f, screenDimensions.y - sprite_ItemShop.getGlobalBounds().height - 25);
	v_Sprites.push_back(sprite_ItemShop);

	sf::Sprite sprite_Inventory(inventory);
	sprite_Inventory.setPosition(screenDimensions.x / 2.0f - sprite_Inventory.getGlobalBounds().width / 2.0f, 75);
	v_Sprites.push_back(sprite_Inventory);

	//*********************************************************************************************Item Sprites

	// Load personal Inentory

	//std::vector<sf::Text> v_Text_Hero_Items;
	/*
	int z = 400;

	for (int i = 0; i < Champion.getEquippedCount(); i++)
	{
		sf::Text heroItems(convertToString(Champion.getEquipmentName(i)), font, 20);
		v_Text_Hero_Items.push_back(heroItems);
		v_Text_Hero_Items.at(i).setPosition(500, z + 25);
		z++;
	}
	*/
	sf::Sprite sprite_BansheesVeil(bansheesVeil);
	sprite_BansheesVeil.setPosition(sprite_ItemShop.getGlobalBounds().left, // Justify to bottom left of item shop
	(sprite_ItemShop.getGlobalBounds().top + sprite_ItemShop.getGlobalBounds().height) - sprite_BansheesVeil.getGlobalBounds().height);
		v_Sprites_Items.push_back(sprite_BansheesVeil);

		sf::Sprite sprite_BladeOfTheRuinedKing(bladeOfTheRuinedKing);
		sprite_BladeOfTheRuinedKing.setPosition(sprite_ItemShop.getGlobalBounds().left, sprite_BansheesVeil.getPosition().y - sprite_BansheesVeil.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_BladeOfTheRuinedKing);

		sf::Sprite sprite_DeadMansPlate(deadMansPlate);
		sprite_DeadMansPlate.setPosition(sprite_ItemShop.getGlobalBounds().left, sprite_BladeOfTheRuinedKing.getPosition().y - sprite_BladeOfTheRuinedKing.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_DeadMansPlate);

		sf::Sprite sprite_FrozenHeart(frozenHeart);
		sprite_FrozenHeart.setPosition(sprite_ItemShop.getGlobalBounds().left + sprite_FrozenHeart.getGlobalBounds().width + 10, sprite_BansheesVeil.getPosition().y);
		v_Sprites_Items.push_back(sprite_FrozenHeart);

		sf::Sprite sprite_FrozenMallet(frozenMallet);
		sprite_FrozenMallet.setPosition(sprite_FrozenHeart.getGlobalBounds().left, sprite_FrozenHeart.getPosition().y - sprite_FrozenHeart.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_FrozenMallet);

		sf::Sprite sprite_InfinityEdge(infinityEdge);
		sprite_InfinityEdge.setPosition(sprite_FrozenHeart.getGlobalBounds().left, sprite_FrozenMallet.getPosition().y - sprite_FrozenMallet.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_InfinityEdge);

		sf::Sprite sprite_LichBane(lichBane);
		sprite_LichBane.setPosition(sprite_FrozenHeart.getGlobalBounds().left + sprite_FrozenHeart.getGlobalBounds().width + 10, sprite_FrozenHeart.getPosition().y);
		v_Sprites_Items.push_back(sprite_LichBane);

		sf::Sprite sprite_LudensEcho(ludensEcho);
		sprite_LudensEcho.setPosition(sprite_FrozenHeart.getGlobalBounds().left + sprite_FrozenHeart.getGlobalBounds().width + 10, sprite_FrozenHeart.getPosition().y - sprite_LudensEcho.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_LudensEcho);

		sf::Sprite sprite_MawOfMalmortius(mawOfMalmortius);
		sprite_MawOfMalmortius.setPosition(sprite_FrozenHeart.getGlobalBounds().left + sprite_FrozenHeart.getGlobalBounds().width + 10, sprite_LudensEcho.getPosition().y - sprite_MawOfMalmortius.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_MawOfMalmortius);

		sf::Sprite sprite_MercurysThreads(mercurysThreads);
		sprite_MercurysThreads.setPosition(sprite_LichBane.getGlobalBounds().left + sprite_LichBane.getGlobalBounds().width + 10, sprite_LichBane.getPosition().y);
		v_Sprites_Items.push_back(sprite_MercurysThreads);

		sf::Sprite sprite_NashorsTooth(nashorsTooth);
		sprite_NashorsTooth.setPosition(sprite_LichBane.getGlobalBounds().left + sprite_LichBane.getGlobalBounds().width + 10, sprite_LichBane.getPosition().y - sprite_NashorsTooth.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_NashorsTooth);

		sf::Sprite sprite_RavenousHydra(ravenousHydra);
		sprite_RavenousHydra.setPosition(sprite_LichBane.getGlobalBounds().left + sprite_RavenousHydra.getGlobalBounds().width + 10, sprite_NashorsTooth.getPosition().y - sprite_RavenousHydra.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_RavenousHydra);

		sf::Sprite sprite_RodOfAges(rodOfAges);
		sprite_RodOfAges.setPosition(sprite_MercurysThreads.getGlobalBounds().left + sprite_MercurysThreads.getGlobalBounds().width + 10, sprite_MercurysThreads.getPosition().y);
		v_Sprites_Items.push_back(sprite_RodOfAges);

		sf::Sprite sprite_SteraksGage(steraksGage);
		sprite_SteraksGage.setPosition(sprite_RodOfAges.getGlobalBounds().left, sprite_RodOfAges.getPosition().y - sprite_SteraksGage.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_SteraksGage);

		sf::Sprite sprite_TheBloodThrister(theBloodthrister);
		sprite_TheBloodThrister.setPosition(sprite_RodOfAges.getGlobalBounds().left, sprite_SteraksGage.getPosition().y - sprite_SteraksGage.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_TheBloodThrister);

		sf::Sprite sprite_ThornMail(thornmail);
		sprite_ThornMail.setPosition(sprite_RodOfAges.getGlobalBounds().left + sprite_RodOfAges.getGlobalBounds().width + 10, sprite_RodOfAges.getPosition().y);
		v_Sprites_Items.push_back(sprite_ThornMail);

		sf::Sprite sprite_TitanicHydra(titanicHydra);
		sprite_TitanicHydra.setPosition(sprite_RodOfAges.getGlobalBounds().left + sprite_RodOfAges.getGlobalBounds().width + 10, sprite_ThornMail.getPosition().y - sprite_ThornMail.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_TitanicHydra);

		sf::Sprite sprite_TrinityForce(trinityForce);
		sprite_TrinityForce.setPosition(sprite_RodOfAges.getGlobalBounds().left + sprite_RodOfAges.getGlobalBounds().width + 10, sprite_TitanicHydra.getPosition().y - sprite_TitanicHydra.getGlobalBounds().height - 10);
		v_Sprites_Items.push_back(sprite_TrinityForce);

		/*
		for (int i = 0; i < Champion.getEquippedCount(); i++)
		{
			if (v_Text_Hero_Items.at(i).getString() == "Banshee's Veil")
				v_Sprites_Items.at(i).setPosition(sprite_Inventory.getGlobalBounds().left + 10, sprite_Inventory.getGlobalBounds().top + 10);
			if (v_Text_Hero_Items.at(i).getString() == "Blade of The Ruined King")
				v_Sprites_Items.at(i).setPosition(sprite_Inventory.getGlobalBounds().left + 85, sprite_Inventory.getGlobalBounds().top + 10);
		}
		*/

		//Table.findName(v_Text_Hero_Items.at(0).getString());
	
	// Audio
	sf::SoundBuffer homeButtonClickBuffer;
	if (!homeButtonClickBuffer.loadFromFile("HomeButtonClick.ogg"))
		window.close();

	sf::Sound homeButtonClick;
	homeButtonClick.setBuffer(homeButtonClickBuffer);


	// Vector to store stat place holders
	std::vector<sf::Text> v_heroStats;

	sf::Text hero_Health("Health:", font, 40);
	sf::Text hero_Mana("Mana:", font, 40);
	sf::Text hero_Attack_Range("Attack range:", font, 40);
	sf::Text hero_Attack_Damage("Attack damage:", font, 40);
	sf::Text hero_Attack_Speed("Attack speed:", font, 40);
	sf::Text hero_Armor("Armor:", font, 40);
	sf::Text hero_Magic_Resistance("Magic resistance:", font, 40);
	sf::Text hero_Movement_Speed("Movement speed:", font, 40);

	// Declare champions data
	sf::Text heroName(convertToString(Champion.getName()), font, 60);
	sf::Text heroHealth(convertToString(Champion.getHealth()), font, 40);
	sf::Text heroMana(convertToString(Champion.getMana()), font, 40);
	sf::Text heroAttackRange(convertToString(Champion.getAttackRange()), font, 40);
	sf::Text heroAttackDamage(convertToString(Champion.getAttackDamage()), font, 40);
	sf::Text heroAttackSpeed(convertToString(Champion.getAttackSpeed()), font, 40);
	sf::Text heroArmor(convertToString(Champion.getarmor()), font, 40);
	sf::Text heroMagicResistance(convertToString(Champion.getMagicResistance()), font, 40);
	sf::Text heroMovementSpeed(convertToString(Champion.getMovementSpeed()), font, 40);

	// Item texts

	std::vector<sf::Text> v_Texts_Item_Stats; // Vector to store data that will be converted in SFML strings
	
	Store_Inventory->begin();
	
	while (!Store_Inventory->off_end())
	{
		sf::Text itemStats(convertToString(Store_Inventory->equipmentString()), font, 20);
		v_Texts_Item_Stats.push_back(itemStats);
		Store_Inventory->scroll();
	}

		for (int i = 0; i < v_Texts_Item_Stats.size(); i++)
			v_Texts_Item_Stats.at(i).setColor(sf::Color::Red);
	
	// Format and set texts position and store in vector

	//center text
	
	sf::FloatRect hero_Name = heroName.getGlobalBounds();
	heroName.setOrigin(hero_Name.left + hero_Name.width / 2.0f,
		hero_Name.top + hero_Name.height / 2.0f);
	heroName.setPosition(sf::Vector2f(screenDimensions.x / 2.0f, 35));

	v_heroStats.push_back(heroName);

	hero_Health.setPosition(0, 0);
	heroHealth.setPosition(hero_Health.getGlobalBounds().width + 10, 0); // Left justified
	
	v_heroStats.push_back(hero_Health);
	v_heroStats.push_back(heroHealth);

	hero_Mana.setPosition(0, hero_Health.getGlobalBounds().top + 50);
	heroMana.setPosition(hero_Mana.getGlobalBounds().width + 10, hero_Health.getGlobalBounds().top + 50);

	v_heroStats.push_back(hero_Mana);
	v_heroStats.push_back(heroMana);

	hero_Attack_Range.setPosition(0, hero_Mana.getGlobalBounds().top + 50);
	heroAttackRange.setPosition(hero_Attack_Range.getGlobalBounds().width + 10, hero_Mana.getGlobalBounds().top + 50);

	v_heroStats.push_back(hero_Attack_Range);
	v_heroStats.push_back(heroAttackRange);

	hero_Attack_Damage.setPosition(0, hero_Attack_Range.getGlobalBounds().top + 50);
	heroAttackDamage.setPosition(hero_Attack_Damage.getGlobalBounds().width + 10, hero_Attack_Range.getGlobalBounds().top + 50);

	v_heroStats.push_back(hero_Attack_Damage);
	v_heroStats.push_back(heroAttackDamage);

	hero_Attack_Speed.setPosition(0, hero_Attack_Damage.getGlobalBounds().top + 50);
	heroAttackSpeed.setPosition(hero_Attack_Speed.getGlobalBounds().width + 10, hero_Attack_Damage.getGlobalBounds().top + 50);
	
	v_heroStats.push_back(hero_Attack_Speed);
	v_heroStats.push_back(heroAttackSpeed);

	hero_Armor.setPosition(0, hero_Attack_Speed.getGlobalBounds().top + 50);
	heroArmor.setPosition(hero_Armor.getGlobalBounds().width + 10, hero_Attack_Speed.getGlobalBounds().top + 50);

	v_heroStats.push_back(hero_Armor);
	v_heroStats.push_back(heroArmor);

	hero_Magic_Resistance.setPosition(0, hero_Armor.getGlobalBounds().top + 50);
	heroMagicResistance.setPosition(hero_Magic_Resistance.getGlobalBounds().width + 10, hero_Armor.getGlobalBounds().top + 50);
	
	v_heroStats.push_back(hero_Magic_Resistance);
	v_heroStats.push_back(heroMagicResistance);

	hero_Movement_Speed.setPosition(0, hero_Magic_Resistance.getGlobalBounds().top + 50);
	heroMovementSpeed.setPosition(hero_Movement_Speed.getGlobalBounds().width + 10, hero_Magic_Resistance.getGlobalBounds().top + 50);
	
	v_heroStats.push_back(hero_Movement_Speed);
	v_heroStats.push_back(heroMovementSpeed);

	for (int i = 0; i < v_heroStats.size(); i++) // Format texts in vector
	{
		v_heroStats.at(i).setColor(sf::Color(32, 178, 206));
	}

	sf::Vector2f HomeButton_defaultPos = sprite_HomeButton.getPosition();
	sf::Vector2f HomeButtonArrow_defaultPos = sprite_HomeButtonArrow.getPosition();

	sf::RectangleShape toolTip;
	toolTip.setSize(sf::Vector2f(300, 150));
	toolTip.setOutlineColor(sf::Color(32, 178, 206));
	toolTip.setFillColor(sf::Color::Black);
	toolTip.setOutlineThickness(5);
	toolTip.setPosition(0, 0);


	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					window.close();
				break;
			case sf::Event::MouseMoved:
				if (sprite_HomeButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					v_Sprites.at(Home_Button).setPosition(HomeButton_defaultPos.x - 100.0f, HomeButton_defaultPos.y);
					v_Sprites.at(Home_Button_Arrow).setPosition(HomeButtonArrow_defaultPos.x - 100.0f, HomeButtonArrow_defaultPos.y);

					v_Sprites.at(Home_Button).setScale(1.5, 1.5);
					v_Sprites.at(Home_Button_Arrow).setScale(1.5, 1.5);

					v_Sprites.at(Home_Button_Arrow).setColor(sf::Color::White);
				}
				else // reset back to orginal
				{
					v_Sprites.at(Home_Button).setPosition(HomeButton_defaultPos.x, HomeButton_defaultPos.y);
					v_Sprites.at(Home_Button_Arrow).setPosition(HomeButtonArrow_defaultPos.x, HomeButtonArrow_defaultPos.y);

					v_Sprites.at(Home_Button).setScale(1, 1);
					v_Sprites.at(Home_Button_Arrow).setScale(1, 1);

					v_Sprites.at(Home_Button_Arrow).setColor(sf::Color::Black);
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (sprite_HomeButton.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					window.clear();
					homeButtonClick.play();
					launchMenu(window, screenDimensions);
				}

				if (sprite_BansheesVeil.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					sprite_BansheesVeil.setPosition(0, 0);
				break;
			case sf::Event::Closed:
				window.close();
				break;
			default:
				break;
			}

			window.clear();

			// Draw sprites
			for (int i = 0; i < v_Sprites.size(); i++)
				window.draw(v_Sprites.at(i));

			for (int i = 0; i < v_Sprites_Items.size(); i++)
			{
				window.draw(v_Sprites_Items.at(i));
				// Set the position of tooltip to be the mouse cooridinates
				if (v_Sprites_Items.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				{
					sf::Vector2f pixel_coord = window.mapPixelToCoords(sf::Mouse::getPosition(window));
					toolTip.setPosition(pixel_coord.x, (pixel_coord.y - toolTip.getGlobalBounds().height) - 200);

					if (i < v_Texts_Item_Stats.size())
						v_Texts_Item_Stats.at(i).setPosition(toolTip.getGlobalBounds().left + 10, toolTip.getGlobalBounds().top + 10);

					window.draw(toolTip);
				}	
			}

			// Draw Texts
			for (int i = 0; i < v_heroStats.size(); i++)
				window.draw(v_heroStats.at(i));

			for (int i = 0; i < v_Texts_Item_Stats.size(); i++)
			{
				// Draw prespective item stats if the mouse is hovering it.
				if (v_Sprites_Items.at(i).getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
					window.draw(v_Texts_Item_Stats.at(i));
			}
			

			window.display();
		}
	}
}
//=========================================================================================================================================League Project functions
//======================================================= getData()
void getData(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* champHealth, BST<int>* equipHealth) {

	Hero current; Weapon weapon; Armor armor; Equipment tempEquip;
	string tempString; int tempInt; double tempDouble; //char tempChar;
	ifstream inFile;

	inFile.open("Champions.txt"); // Load Chmapion data
	if (inFile.fail()) {
		cout << "Error! Champions data not found!" << endl;
		return;
	}
	else {

		while (!inFile.eof()) {
			inFile >> tempString;
			current.setName(tempString);
			Table->addItem(tempString, Champions->getIndex());
			inFile >> tempInt;
			current.setHealth(tempInt);
			champHealth->add(tempInt, Champions->getIndex());
			inFile >> tempInt;
			current.setMana(tempInt);
			inFile >> tempInt;
			current.setAttackRange(tempInt);
			inFile >> tempInt;
			current.setAttackDamage(tempInt);
			inFile >> tempDouble;
			current.setAttackSpeed(tempDouble);
			inFile >> tempDouble;
			current.setarmor(tempDouble);
			inFile >> tempDouble;
			current.setMagicResistance(tempDouble);
			inFile >> tempInt;
			current.setMovementSpeed(tempInt);
			//current.zeroEquippedCount();
			Champions->insert(current);
			inFile.ignore();
		}//While Loop END
	}

	inFile.close();


	inFile.open("Inventory.txt"); // Load inventory data
	if (inFile.fail()) {
		cout << "Error! Inventory Data not Found!" << endl;
		return;
	}
	else {

		while (!inFile.eof()) {
			inFile >> tempString;

			if (tempString == "Armor") {

				inFile >> tempDouble;
				armor.setArmorBoost(tempDouble);
				inFile >> tempInt;
				armor.setHealthBoost(tempInt);
				equipHealth->add(tempInt, Inventory->getIndex());
				inFile >> tempInt;
				armor.setManaBoost(tempInt);
				inFile >> tempDouble;
				armor.setMagicResistanceBoost(tempDouble);
				inFile.ignore();
				getline(inFile, tempString);
				Table->addItem(tempString, Inventory->getIndex());
				tempEquip.setName(tempString);
				tempEquip.setArmor(armor);
				Inventory->insert(tempEquip);
			}
			else {

				inFile >> tempInt;
				weapon.setAttackDamageBoost(tempInt);
				inFile >> tempDouble;
				weapon.setattackSpeedBoost(tempDouble);
				inFile.ignore();
				getline(inFile, tempString);
				Table->addItem(tempString, Inventory->getIndex());
				tempEquip.setName(tempString);
				tempEquip.setWeapon(weapon);
				Inventory->insert(tempEquip);
			}

		}
	}
	inFile.close();

	inFile.open("Personal_Inventory.txt");
	if (inFile.fail()) {
		cout << "Personal_Inventory.txt failed to open\n";
	}
	else {
		while (!inFile.eof()) {

			inFile >> tempString;
			int index = Table->findName(tempString);
			Champions->scrollToIndex(index);
			inFile >> tempInt;
			Hero old = Champions->current();

			inFile.ignore();
			for (int i = 0; i < tempInt; i++) {
				getline(inFile, tempString);
				Inventory->scrollToIndex(Table->findName(tempString));
				tempEquip = Equipment(Inventory->current()); // Null index
				Champions->equipCurrent(tempEquip);
			}
			Hero current = Champions->current();
			champHealth->containReplace(old.getHealth(), current.getHealth());
		}
	}
	inFile.close();


}// getData Function END
 //======================================================= reHashChampions(Champions, Table)
void reHashChampions(List<Hero> Champions, HashTable* Table) {

	Champions.begin(); Hero champTemp;

	while (!Champions.off_end()) {
		champTemp = Champions.current();
		Table->updateIndex(champTemp.getName(), Champions.getIndex());

		Champions.scroll();
	}
}
//======================================================= reHashInventory(Inventory, Table)
void reHashInventory(List<Equipment> Inventory, HashTable* Table) {
	Inventory.begin(); Equipment tempEquip;

	while (!Inventory.off_end()) {
		tempEquip = Inventory.current();
		Table->updateIndex(tempEquip.getName(), Inventory.getIndex());
		Inventory.scroll();
	}
}

//======================================================= mainMenu()
char mainMenu() {
	char choice;

	cout << endl;
	cout << "\t\t\t\t ** Welcome to the Main Menu **\n\nPlease choose from the following:\n\n";

	cout << "\t C to enter the Champions Lounge\n"
		<< "\t E to enter the Equipment room\n"
		<< "\t Q to exit program";


	cout << "\n\nChoice: "; cin >> choice;
	return toupper(choice);

	return toupper(choice);
}
//======================================================= championHallMenu()
char championHallMenu() {
	char choice = 'E';
	cout << endl;
	cout << "\t\t\t\t ** Welcome to the Champions Lounge **\n\nPlease choose from the following:\n\n";

	cout << "\t A to add new champion\n"
		<< "\t B to go to battle\n"
		<< "\t S to enter search menu\n"
		<< "\t P to print champion list\n"
		<< "\t R to remove a champion\n"
		<< "\t E to exit the Lounge";


	cout << "\n\nChoice: "; cin >> choice;
	return toupper(choice);
}// mainMenu() function END

 //======================================================= searchMenu()
char searchMenu() {

	char choice = 'E';
	cout << endl;
	cout << "\t\t\t\t\t *** Search Menu ***\n\nPlease choose from the following:\n\n";

	cout << "\t N to search by name\n"
		<< "\t A to search by Attribute\n"
		<< "\t E to exit the search menu\n";

	cout << "\n\nChoice: ";
	cin >> choice;
	//cout << "------------------------------------------------------\n\n";

	return toupper(choice);
}
//======================================================= attributeMenu()
char attributeMenu() {
	char choice = 'E';
	cout << endl;
	cout << "\t\t\t\t *** Attribute Menu ***\n\nPlease choose from the following:\n\n";

	cout << "\t H to search by Health\n"
		<< "\t M to search by Mana\n"
		<< "\t R to search by Attack Range\n"
		<< "\t D to search by Attack Damage\n"
		<< "\t S to search by Attack Speed\n"
		<< "\t A to search by Armor \n"
		<< "\t G to search by Magic Resistance\n"
		<< "\t Q to search by Movement Speed\n"
		<< "\t E to exit the attribute menu";

	cout << "\n\nChoice: ";
	cin >> choice;

	return toupper(choice);

}
//======================================================= equipmentRoom()
char equipmentRoomMenu() {
	char choice = 'E';
	cout << endl;
	cout << "\t\t\t\t *** Equipment Room ***\n\nPlease choose from the following:\n\n";

	cout << "\t Q to equip a champion\n"
		<< "\t D to remove champion equipment\n"
		<< "\t A to add new item\n"
		<< "\t S to search for equipment\n"
		<< "\t R to destroy an item\n"
		<< "\t P to print inventory List\n"
		<< "\t E to exit the Equipment Room\n";

	cout << "\n\nChoice: ";
	cin >> choice;


	return toupper(choice);
}

//======================================================= addChampion()
void addChampion(List<Hero>* Champions, HashTable* Table, BST<int>* tree) {
	Hero newChampion; string tempString; int tempInt; double tempDouble; int count; char choice;

	//creating random generator

	Random r;  // Example of how to generator a number between 1-200 -> r.DrawNumber(1, 200);

	cout << "Would you like to use the Random Stat Generator? (Y/N): ";
	cin >> choice;

	if (toupper(choice) == 'Y') {

		cout << "How many champions would you like to add?: ";
		cin >> count;

		for (int i = 0; i < count; i++) {

			cout << "\nEnter champion's Name: ";
			cin >> tempString;
			newChampion.setName(tempString);

			newChampion.setHealth(r.DrawNumber(450, 650));
			cout << "Health: " << newChampion.getHealth() << endl;
			tree->add(newChampion.getHealth(), Champions->getIndex() + 1);

			newChampion.setMana(r.DrawNumber(235, 365));
			cout << "Mana: " << newChampion.getMana() << endl;

			newChampion.setAttackRange(r.DrawNumber(150, 565));
			cout << "Attack Range: " << newChampion.getAttackRange() << endl;

			newChampion.setAttackDamage(r.DrawNumber(38, 60));
			cout << "Attack Damage: " << newChampion.getAttackDamage() << endl;

			newChampion.setAttackSpeed(r.DrawNumber(600, 800) * .001); // Convert the random generated int into a double.
			cout << "Attack Speed: " << newChampion.getAttackSpeed() << endl;

			newChampion.setarmor(r.DrawNumber(15, 35));
			cout << "Armor: " << newChampion.getarmor() << endl;

			newChampion.setMagicResistance(r.DrawNumber(25, 35));
			cout << "Magic Resistance: " << newChampion.getMagicResistance() << endl;

			newChampion.setMovementSpeed(r.DrawNumber(315, 375));
			cout << "Movement Speed: " << newChampion.getMovementSpeed() << endl;

			Champions->push_back(newChampion);
			Table->addItem(tempString, Champions->getIndex());
			cout << i + 1 << " of " << count << " added." << endl;

		}// for loop END
	}// if statement END

	else {

		cout << "How many champions would you like to add?: ";
		cin >> count;

		for (int i = 0; i < count; i++) {
			cout << "\nEnter champion's Name: ";

			cin >> tempString;
			newChampion.setName(tempString);

			cout << "\nEnter champion's Health: ";
			cin >> tempInt;
			newChampion.setHealth(tempInt);


			cout << "\nEner champion's Mana: ";
			cin >> tempInt;
			newChampion.setMana(tempInt);

			cout << "\nEnter champion's Damage Range: ";
			cin >> tempInt;
			newChampion.setAttackRange(tempInt);

			cout << "\nEnter champion's Attack Damage: ";
			cin >> tempInt;
			newChampion.setAttackDamage(tempInt);


			cout << "\nEnter champion's Attack Speed: ";
			cin >> tempDouble;
			newChampion.setAttackSpeed(tempDouble); // The random number generator only works with ints


			cout << "\nEnter champion's Armor: ";
			cin >> tempDouble;
			newChampion.setarmor(tempDouble);

			cout << "\nEnter champion's Magic Resistance: ";
			cin >> tempDouble;
			newChampion.setMagicResistance(tempDouble);

			cout << "\nEnter champion's Movement Speed: ";
			cin >> tempInt;
			newChampion.setMovementSpeed(tempInt);

			Champions->push_back(newChampion);

			cout << i + 1 << " of " << count << " added." << endl;
		}// for Loop END

	}// else statement END
	return;
}// addChampion Function END
 //======================================================= addEquipment()
void addEquipment(List<Equipment>* Inventory, HashTable* Table, BST<int>* tree) {
	Equipment newEquipment; string tempString; int tempInt; double tempDouble; int count; char choice;

	//creating random generator

	Random r;  // Example of how to generator a number between 1-200 -> r.DrawNumber(1, 200);

	cout << "\nWould you like to use the Random Stat Generator? (Y/N): ";
	cin >> choice;

	if (toupper(choice) == 'Y') {
		cout << "\nHow many items would you like to add?: ";
		cin >> count;

		for (int i = 0; i < count; i++) {
			cin.ignore();
			cout << "\nEnter Equipment Name: ";
			getline(cin, tempString);
			newEquipment.setName(tempString);

			cout << "\nEnter 1 for Weapon or 2 for Armor";
			cin >> tempInt; cout << endl;

			if (tempInt == 1) {
				newEquipment.setType("Weapon");
			}
			else if (tempInt == 2) {
				newEquipment.setType("Armor");
			}
			else {
				cout << tempInt << " Not a valid option!";
				exit(-1);
			}

			if (newEquipment.getType() == "Weapon") {
				Weapon tempWeapon;
				tempWeapon.setAttackDamageBoost(r.DrawNumber(10, 100));
				tempWeapon.setattackSpeedBoost(r.DrawNumber(450, 990)*0.0001);

				newEquipment.setWeapon(tempWeapon);
				Inventory->insert(newEquipment);
				Table->addItem(tempString, Inventory->getIndex() - 1);


			}
			else {
				Armor tempArmor;
				tempArmor.setArmorBoost(r.DrawNumber(400, 1200)*0.127);
				tempArmor.setMagicResistanceBoost(r.DrawNumber(200, 1000)*0.124);
				tempArmor.setManaBoost(r.DrawNumber(20, 275));
				tempArmor.setHealthBoost(r.DrawNumber(100, 450));

				newEquipment.setArmor(tempArmor);
				Inventory->insert(newEquipment);
				Table->addItem(tempString, Inventory->getIndex() - 1);

			}

			cout << i + 1 << " of " << count << " added." << endl;

		}// for loop END

	}
	else {
		cout << "\nHow many items would you like to add?: ";
		cin >> count;

		for (int i = 0; i < count; i++) {
			cin.ignore();
			cout << "\nEnter Equipment Name: ";
			getline(cin, tempString);
			newEquipment.setName(tempString);

			cout << "\nEnter 1 for Weapon or 2 for Armor";
			cin >> tempInt; cout << endl;

			if (tempInt == 1) {
				newEquipment.setType("Weapon");
			}
			else if (tempInt == 2) {
				newEquipment.setType("Armor");
			}
			else {
				cout << tempInt << " Not a valid option!";
				exit(-1);
			}
			if (newEquipment.getType() == "Weapon") {
				Weapon tempWeapon;
				cout << "\nEnter Attack Damage Boost: ";
				cin >> tempInt;
				tempWeapon.setAttackDamageBoost(tempInt);

				cout << "\nEnter Attack Speed Boost: ";
				cin >> tempDouble;
				tempWeapon.setattackSpeedBoost(tempDouble);

				newEquipment.setWeapon(tempWeapon);
				Inventory->insert(newEquipment);
				Table->addItem(tempString, Inventory->getIndex() - 1);


			}
			else {
				Armor tempArmor;
				cout << "\nEnter Armor Boost: ";
				cin >> tempDouble;
				tempArmor.setArmorBoost(tempDouble);

				cout << "\nEnter Magic Resistance Boost: ";
				cin >> tempDouble;

				tempArmor.setMagicResistanceBoost(tempDouble);

				cout << "\nEnter Mana Boost: ";
				cin >> tempInt;

				tempArmor.setManaBoost(tempInt);

				cout << "\nEnter Health Boost: ";
				cin >> tempInt;

				tempArmor.setHealthBoost(tempInt);

				newEquipment.setArmor(tempArmor);
				Inventory->insert(newEquipment);
				Table->addItem(tempString, Inventory->getIndex() - 1);
				tree->add(tempInt, Inventory->getIndex());

			}

			cout << i + 1 << " of " << count << " added." << endl;


		}//END for loop
	}



}

//======================================================= print_to_file()
void print_to_file(List<Hero> Champions, List<Equipment> Inventory) {

	ofstream outFile; Hero tempChampion; int count = 0;
	Equipment tempEquip; Weapon tempWeapon; Armor tempArmor;


	outFile.open("Personal_Inventory.txt");
	if (outFile.fail()) {
		cout << "Error opening Personal_Inventory.txt\n";
	}
	else {
		Champions.begin();

		while (!Champions.off_end()) {
			tempChampion = Champions.current();

			if (tempChampion.getEquippedCount() != 0) {

				if (count != 0) {// newline if not first input
					outFile << "\n";
				}

				outFile << tempChampion.getName() << " ";
				outFile << tempChampion.getEquippedCount();

				for (int i = 0; i < tempChampion.getEquippedCount(); i++) {
					tempEquip = Equipment(Champions.sendCurrentEquip(i));
					outFile << endl << tempEquip.getName();
				}

				count++;

			}
			Champions.scroll();

		}
		cout << count << " Champion Inventory Saved.\n";
	}
	outFile.close();

	Champions.dequipAll();

	outFile.open("Champions.txt");
	if (outFile.fail()) {
		cout << "Error opening file! " << endl;
	}
	else
	{
		Champions.begin(); count = 0;

		while (!Champions.off_end()) {

			tempChampion = Champions.current(); // Make champion data accessible
			if (count != 0) // Newline if not first input
				outFile << "\n";

			outFile << tempChampion.getName() << " " << tempChampion.getHealth() << " " << tempChampion.getMana() << " "
				<< tempChampion.getAttackRange() << " " << tempChampion.getAttackDamage() << " " << tempChampion.getAttackSpeed()
				<< " " << tempChampion.getarmor() << " " << tempChampion.getMagicResistance() << " " << tempChampion.getMovementSpeed();

			Champions.scroll();
			count++;
		}//While Loop END

		cout << count << " Champions saved. \n";
	}// else statement END
	outFile.close();

	outFile.open("Inventory.txt");
	if (outFile.fail()) {
		cout << "Inventory.txt unable to open.\n";
	}
	else {
		count = 0;
		Inventory.begin();

		while (!Inventory.off_end()) {
			tempEquip = Inventory.current();

			if (count != 0) {
				outFile << "\n";
			}
			outFile << tempEquip.getType() << " ";
			if (tempEquip.getType() == "Weapon") {
				tempWeapon = tempEquip.getWeapon();
				outFile << tempWeapon.getAttackDamage() << " "
					<< tempWeapon.getAttackSpeed() << " ";



			}
			else {
				tempArmor = tempEquip.getArmor();
				outFile << tempArmor.getArmorBoost() << " "
					<< tempArmor.getHealthBoost() << " "
					<< tempArmor.getManaBoost() << " "
					<< tempArmor.getMagicResistanceBoost() << " ";

			}
			outFile << tempEquip.getName();
			Inventory.scroll(); count++;
		}
		cout << count << " Inventory items saved.\n";
	}
	outFile.close();


}// print_to_file Function END
 //======================================================= search()
void search(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree, bool remove, char typeSwitch) {
	char choice; string query; Hero temp; Equipment eTemp; int selected = 0; char response = 'N'; bool found = false; char attribute;
	int intMin, intMax; double doubleMin, doubleMax; int counted = 0; int convert = 0; vector<int> indexRange;

	while ((choice = searchMenu()) && choice != 'E') {
		found = false;


		switch (choice) {
		case 'N':// Name

			if (typeSwitch == 'C') {
				cout << "\nEnter champion name: ";
				cin >> query; cout << endl;

				Champions->scrollToIndex(Table->findName(query));
				selected = 0;

				if (Champions->off_end() == false) {

					temp = Champions->current();
					selected = Table->findName(query);// this keeps track of where in the list we are. If query is found, selected is the spot in the list that remove function must delete;
					temp.print(); found = true;

					//====================================== Remove option enabled
					if (remove == true) {
						cout << "\nWould you like to delete? (Y/N): ";
						cin >> response;
						if (toupper(response) == 'N') {
							selected = 0;
						}// if response == 'N' statement END
						Table->removeItem(query);
					}// if (remove==true) statement END
					break;



				}// if statement END
				if (found == false)
					cout << "\nNo match found!" << endl;


			}
			else if (typeSwitch == 'I') {
				cin.ignore();
				cout << "\nEnter Item name: ";

				getline(cin, query); cout << endl;

				Inventory->scrollToIndex(Table->findName(query)); selected = 0;


				if (Inventory->off_end() == false) {

					eTemp = Inventory->current();
					selected = Table->findName(query);// this keeps track of where in the list we are. If query is found, selected is the spot in the list that remove function must delete;
					eTemp.print(); found = true;

					//====================================== Remove option enabled
					if (remove == true) {
						cout << "\nWould you like to delete? (Y/N): ";
						cin >> response;
						if (toupper(response) == 'N') {
							selected = 0;
						}// if response == 'N' statement END
						Table->removeItem(query);
					}// if (remove==true) statement END
					break;


				}// if Statement END
				if (found == false)
					cout << "\nNo match found!" << endl;
			}

			break;// End Name

		case 'A':// Attribute
			Champions->begin(); selected = 0; counted = 0;
			cout << "Select an attribute:\n";
			//---------------------------------------------------------- Search Attribute begin ---------------------------------------------------------
			if (typeSwitch == 'C') {

				switch ((attribute = attributeMenu())) {

				case 'H':// Health
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;


					indexRange = tree->search(intMin, intMax);


					for (int i = 0; i < indexRange.size(); i++) {
						found = true;
						Champions->scrollToIndex(indexRange.at(i));
						temp = Champions->current();
						cout << endl << i + 1 << endl;
						temp.print();

					}


					//====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->scrollToIndex(indexRange.at(convert - 1));
							selected = Champions->getIndex() - 1;
							cout << "Selected: " << selected;
						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;// END Health

				case 'M':// Mana
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();
						if (temp.getMana() >= intMin && temp.getMana() <= intMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;

						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getMana() >= intMin && temp.getMana() <= intMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;

					break;

				case 'R':// Attack Range
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getAttackRange() >= intMin && temp.getAttackRange() <= intMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;


						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getAttackRange() >= intMin && temp.getAttackRange() <= intMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'D':// Attack Damage
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getAttackDamage() >= intMin && temp.getAttackDamage() <= intMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;


						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getHealth() >= intMin && temp.getHealth() <= intMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'S':// Attack Speed
					cout << "Enter Min Range: "; cin >> doubleMin;
					cout << "\nEnter Max Range: "; cin >> doubleMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getAttackSpeed() >= doubleMin && temp.getAttackSpeed() <= doubleMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;


						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getAttackSpeed() >= doubleMin && temp.getAttackSpeed() <= doubleMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'A':// armor
					cout << "Enter Min Range: "; cin >> doubleMin;
					cout << "\nEnter Max Range: "; cin >> doubleMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getarmor() >= doubleMin && temp.getarmor() <= doubleMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;


						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getarmor() >= doubleMin && temp.getarmor() <= doubleMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'G':// Magic Resistance
					cout << "Enter Min Range: "; cin >> doubleMin;
					cout << "\nEnter Max Range: "; cin >> doubleMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getMagicResistance() >= doubleMin && temp.getMagicResistance() <= doubleMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;

						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getMagicResistance() >= doubleMin && temp.getMagicResistance() <= doubleMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'Q':// Movement Speed
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Champions->off_end() == false) {
						temp = Champions->current();

						if (temp.getMovementSpeed() >= intMin && temp.getMovementSpeed() <= intMax) {
							counted++;
							cout << endl << endl << counted << endl; temp.print(); found = true;


						}//if Statement END

						Champions->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Champions->begin(); counted = 0; selected = 0;
							while (Champions->off_end() == false && counted < convert) {
								temp = Champions->current();
								selected++;
								if (temp.getMovementSpeed() >= intMin && temp.getMovementSpeed() <= intMax) {
									counted++;
								}//if Statement END

								Champions->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				default:
					cout << endl << choice << " is not a valid choice!\n";
					break;
				}// End Attribute switch

				 //---------------------------------------------------------- Search Attribute Else ---------------------------------------------------------
			}
			else if (typeSwitch == 'I') {
				Inventory->begin();

				switch ((attribute = attributeMenu())) {

				case 'D'://attack damage Boost
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Inventory->off_end() == false) {

						eTemp = Inventory->current();

						if (eTemp.getType() == "Weapon") {
							Weapon wpn2compare(eTemp.getWeapon());


							if (wpn2compare.getAttackDamage() >= intMin && wpn2compare.getAttackDamage() <= intMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}


						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;


							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Weapon") {
									Weapon wpn2compare = eTemp.getWeapon();
									selected++;
									if (wpn2compare.getAttackDamage() >= intMin && wpn2compare.getAttackDamage() <= intMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;

					break;

				case 'S':// Attack Speed
					cout << "Enter Min Range: "; cin >> doubleMin;
					cout << "\nEnter Max Range: "; cin >> doubleMax;

					while (Inventory->off_end() == false) {
						eTemp = Inventory->current();

						if (eTemp.getType() == "Weapon") {
							Weapon wpn2compare(eTemp.getWeapon());

							if (wpn2compare.getAttackSpeed() >= doubleMin && wpn2compare.getAttackSpeed() <= doubleMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}
						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;
							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Weapon") {
									Weapon wpn2compare = eTemp.getWeapon();
									selected++;
									if (wpn2compare.getAttackDamage() >= doubleMin && wpn2compare.getAttackDamage() <= doubleMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'H':// Health Boost
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Inventory->off_end() == false) {
						eTemp = Inventory->current();

						if (eTemp.getType() == "Armor") {
							Armor arm2compare(eTemp.getArmor());

							if (arm2compare.getHealthBoost() >= intMin && arm2compare.getHealthBoost() <= intMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}
						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;
							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Armor") {
									Armor arm2compare = eTemp.getArmor();
									selected++;
									if (arm2compare.getHealthBoost() >= intMin && arm2compare.getHealthBoost() <= intMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'A':// Armor Boost
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Inventory->off_end() == false) {
						eTemp = Inventory->current();
						if (eTemp.getType() == "Armor") {
							Armor arm2compare(eTemp.getArmor());

							if (arm2compare.getArmorBoost() >= intMin && arm2compare.getArmorBoost() <= intMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}
						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;
							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Armor") {
									Armor arm2compare = eTemp.getArmor();
									selected++;
									if (arm2compare.getArmorBoost() >= intMin && arm2compare.getArmorBoost() <= intMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'M': // Mana Boost
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Inventory->off_end() == false) {
						eTemp = Inventory->current();
						if (eTemp.getType() == "Armor") {
							Armor arm2compare(eTemp.getArmor());

							if (arm2compare.getManaBoost() >= intMin && arm2compare.getManaBoost() <= intMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}
						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;
							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Armor") {
									Armor arm2compare = eTemp.getArmor();
									selected++;
									if (arm2compare.getManaBoost() >= intMin && arm2compare.getManaBoost() <= intMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				case 'G': // Magic Resistance Boost
					cout << "Enter Min Range: "; cin >> intMin;
					cout << "\nEnter Max Range: "; cin >> intMax;

					while (Inventory->off_end() == false) {
						eTemp = Inventory->current();
						if (eTemp.getType() == "Armor") {
							Armor arm2compare(eTemp.getArmor());

							if (arm2compare.getMagicResistanceBoost() >= intMin && arm2compare.getMagicResistanceBoost() <= intMax) {
								counted++;
								cout << endl << endl << counted << endl; eTemp.print(); found = true;


							}//if Statement END
						}

						Inventory->scroll();

					}// while loop END

					 //====================================== Remove option enabled
					if (remove == true && found == true) {
						cout << "\nWhich you like to delete? (Enter 0 to cancel): ";
						cin >> convert;
						if (convert == 0) {
							selected = 0;
						}
						else
						{
							Inventory->begin(); counted = 0; selected = 0;
							while (Inventory->off_end() == false && counted < convert) {
								eTemp = Inventory->current();
								if (eTemp.getType() == "Armor") {
									Armor arm2compare = eTemp.getArmor();
									selected++;
									if (arm2compare.getMagicResistanceBoost() >= intMin && arm2compare.getMagicResistanceBoost() <= intMax) {
										counted++;
									}//if Statement END
								}
								Inventory->scroll();

							}// while loop END

						}
					}// if (remove==true) statement END


					if (found == false)
						cout << "\nNo match found!" << endl;
					break;

				default:
					cout << endl << attribute << " is not a valid choice!\n";;
					break;


				}// switch inventory attribute


			}
			else
				cout << "Invalid Seach choice\n";




			break;// End Attribute

		case 'E'://Exit
			return;

			break;

		default:
			cout << "\nNot a valid choice!\n";
			break;
		}


		//======================================================================== Remove option enabled
		if (remove == true && selected > 0 && typeSwitch == 'C') {

			Champions->scrollToIndex(selected);
			Champions->remove();


		}
		else if (remove == true && selected > 0 && typeSwitch == 'I') {
			Inventory->scrollToIndex(selected);
			Inventory->remove();
		}

	}// while statement END
}
//======================================================= battle()
void battle(List<Hero>* Champions) {
	string selectedChampion; int selected; Hero temp; bool found = false;
	cout << "Enter the name of the champion you want to fight with: " << endl;
	cin >> selectedChampion;

	Champions->begin(); selected = 0;

	while (Champions->off_end() == false) {

		temp = Champions->current();
		selected++;// this keeps track of where in the list we are. If query is found, selected is the spot in the list that remove function must delete;
		if (temp.getName() == selectedChampion) {
			//temp.print();
			found = true;
		}
	}//end while
	if (found == true)
	{
		cout << "true" << endl;
	}
	else { cout << "false" << endl; }

}
//======================================================= EquipmentRoom()
void equipmentRoom(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree) {

	char choice; bool remove; char typeSwitch = 'I';

	while ((choice = equipmentRoomMenu()) && choice != 'E') {

		switch (choice) {

		case 'Q':

			equipChampion(Champions, *Inventory);

			break;

		case 'D':

			dequipChampion(Champions);

			break;

		case 'R':
			remove = true;
			search(Champions, Inventory, Table, tree, remove, typeSwitch);

			break;

		case 'S'://Search equipment
			remove = false;
			search(Champions, Inventory, Table, tree, remove, typeSwitch);

			break;

		case 'P':

			Inventory->print();

			break;

		case 'A':// add item
			addEquipment(Inventory, Table, tree);

			break;


		default:
			cout << endl << choice << " is not a valid choice!\n";

			break;

		}// End Switch

	}// End While


}
//======================================================= Champion Hall
void championHall(List<Hero>* Champions, List<Equipment>* Inventory, HashTable* Table, BST<int>* tree) {
	char typeSwitch = 'C'; bool remove; char choice;




	while ((choice = championHallMenu()) && choice != 'E') {

		switch (choice) {
		case 'A':
			addChampion(Champions, Table, tree);
			break;
		case 'B':
			battle(Champions);
			break;
		case 'R':
			remove = true; typeSwitch = 'C';
			search(Champions, Inventory, Table, tree, remove, typeSwitch);
			break;

		case 'S':
			remove = false; typeSwitch = 'C';
			search(Champions, Inventory, Table, tree, remove, typeSwitch);
			break;

		case 'P':
			Champions->print();
			break;


		default:
			cout << endl << choice << "is not a valid choice!\n";
			break;
		}


	}
}

//======================================================= equipChampion()
void equipChampion(List<Hero>* Champions, List<Equipment> Inventory) {

	Equipment tempEquipment; int choice; Hero tempHero;

	Champions->print();

	cout << "\nEnter Champion to equip: ";
	cin >> choice;
	cout << endl << endl;
	Champions->begin();
	for (int i = 0; i < choice - 1; i++) {
		Champions->scroll();
	}


	Inventory.print();

	cout << "\nEnter item to equip: ";
	cin >> choice;
	cout << endl << endl;

	Inventory.begin();

	for (int i = 0; i < choice - 1; i++) {
		Inventory.scroll();
	}

	tempEquipment = Equipment(Inventory.current());

	Champions->equipCurrent(tempEquipment);




}
//======================================================= dequipChampion()
void dequipChampion(List<Hero>* Champions) {

	Champions->print(); int choice = 0; int count = 0;

	cout << "\nEnter Champion Number: ";
	cin >> choice;
	cout << endl << endl;

	Champions->begin();
	for (int i = 0; i < choice - 1; i++) {
		Champions->scroll();
	}

	Champions->print1();

	cout << "\nEnter item to remove: ";
	cin >> count;
	cout << endl;

	Champions->dequipCurrent(count);


}