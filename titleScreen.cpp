#include "title.hpp"

//This function displays the main title window for players while the machine is waiting.
//It displays the title, some screensaver animation, and a prompt to insert a coin (or payment method) to begin play.
int titleWindow(sf::RenderWindow &window) {
	/*
		Monitor Sizes for 16:9
		1280x720 (720p)
		1920x1080 (1080p)
		2560x1440 (2k)
		3840x2160 (4k)
	*/

	window.clear();
	
	//Start clock to monitor elapsed time. It is MONOTONIC, does not go by system clock.
	const auto start{std::chrono::steady_clock::now()};
	
	sf::Font title_font, give_money_font;
	if (!title_font.openFromFile("assets/fonts/TECHNOID.TTF")) { std::perror("File not found!"); }
	if (!give_money_font.openFromFile("assets/fonts/square_sans_serif_7.ttf")) { std::perror("File not found!"); }

	//Title text to be displayed at the top of the screen. Does not flash.
	sf::Text title_text(title_font, "Air Hockey Arcade", 75);
	const sf::FloatRect titleRect = title_text.getLocalBounds();
	title_text.setOrigin(titleRect.getCenter());
	title_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 5.0f));

	//Insert coin to be displayed at bottom of screen. Flashes.
	sf::Text play_text(give_money_font, "Insert Coin to Play!", 35);
	const sf::FloatRect textRect = play_text.getLocalBounds();
	play_text.setOrigin(textRect.getCenter());
	play_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 1.2f));

	//Main window loop
	while ( window.isOpen() ) {
		//Takes the stack and pops each event. We only care about the window closing for now.
		while (std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();
		window.draw(title_text);

		//Takes the elapsed time set at the beginning of function, and subtracts it from the time now.
		//Takes mod of the difference, and chooses to display the text based on whether time is even or odd
		const auto end{std::chrono::steady_clock::now()};
		const auto elapsed_time{end - start};
		const auto sc{std::chrono::duration_cast<std::chrono::seconds>(elapsed_time)};
		long long sec = sc.count();

		if ((sec = static_cast<unsigned int>(sec)) % 2 == 0) {
			window.draw(play_text);
		}

		window.display();
	}

    return 1;
}
