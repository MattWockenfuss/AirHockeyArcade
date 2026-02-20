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
	const sf::FloatRect titleRect = title_text.getLocalBounds();
	title_text.setOrigin((titleRect.left + titleRect.width / 2.0f), (titleRect.top + titleRect.height / 2.0f));
	title_text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 5.0f);

	sf::Text play_text("Insert Coin to Play!", give_money_font, 35);
	const sf::FloatRect textRect = play_text.getLocalBounds();
	play_text.setOrigin((textRect.left + textRect.width / 2.0f), (textRect.top + textRect.height / 2.0f));
	play_text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 1.2f);

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
