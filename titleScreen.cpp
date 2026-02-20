#include "title.hpp"

int titleWindow(sf::RenderWindow &window) {
	/*
		Monitor Sizes for 16:9
		1280x720 (720p)
		1920x1080 (1080p)
		2560x1440 (2k)
		3840x2160 (4k)
	*/

	window.clear();
	
	const auto start{std::chrono::steady_clock::now()};
	
	sf::Font title_font, give_money_font;
	title_font.loadFromFile("assets/fonts/TECHNOID.TTF");
	give_money_font.loadFromFile("assets/fonts/square_sans_serif_7.ttf");

	sf::Text title_text("Air Hockey Arcade", title_font, 75);
	title_text.setPosition(window.getSize().x/5.5, window.getSize().y/5);
	title_text.setStyle(sf::Text::Bold);

	sf::Text play_text("Insert Coin to Play!", give_money_font, 35);
	play_text.setPosition(window.getSize().x/3, window.getSize().y/1.2);

	while ( window.isOpen() ) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(title_text);

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
