#define BUBBLE 0
#define SELECTION 1
#define HEAP 2
#define MERGE 3

#include <SFML/Graphics.hpp>
#include <assert.h>

namespace kku {

    const int Kinds_sortings = 4;
    const int Kinds_fillings = 1;

    void is_active(sf::RectangleShape *button, bool active) {
        button->setOutlineColor((active) ? sf::Color::Green : sf::Color(140, 140, 140));
    }


    bool check_border(sf::RectangleShape border, sf::RenderWindow *window) {
        return (border.getPosition().x <= sf::Mouse::getPosition(*window).x
                && sf::Mouse::getPosition(*window).x <= border.getPosition().x + border.getSize().x
                && border.getPosition().y <= sf::Mouse::getPosition(*window).y
                && sf::Mouse::getPosition(*window).y <= border.getPosition().y + border.getSize().y);
    }

    void edit_text(sf::Text *text, std::string write, sf::Font *font) {
        text->setFont(*font);
        text->setString(write);
        text->setColor(sf::Color::Black);
        text->setCharacterSize(18);
    }

    void edit_arr_texts(int cnt_of_elements, sf::Text text[], std::string write[], sf::Font *font) {
        for (int i = 0; i < cnt_of_elements; i++) {
            edit_text(&text[i], write[i], font);
        }
    }

    void edit_button(sf::RectangleShape *but) {
        but->setFillColor(sf::Color(245, 245, 245));
        but->setOutlineThickness(3);
        but->setOutlineColor(sf::Color(140, 140, 140));
        but->setSize(sf::Vector2f(160, 30));
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
            "Analys sortings");
    sf::RenderWindow graph1; //"Dependence of the number of permutations from N"
    sf::RenderWindow graph2; //"Dependence of the number of comparisons from N"

    sf::View fixed = window.getView();
    sf::Texture texture_pointer;
    texture_pointer.loadFromFile("../pointer.png");

    sf::Sprite pointer(texture_pointer);

    sf::Font font;

    if (!font.loadFromFile("../Roboto-Regular.ttf"))
        return 1;

    sf::Text brief;

    kku::edit_text(&brief,
                   "\n\t" "There's you can analys sortings. Choose the kind of sortings and the kind of fillings",
                   &font);

    sf::Text choose_sort;

    kku::edit_text(&choose_sort, "\n\t" "Kinds of sortings:", &font);
    choose_sort.setPosition(brief.getPosition().x, brief.getPosition().y + 40);

    sf::Text names_of_sortings[kku::Kinds_sortings];
    std::string names_sorts[kku::Kinds_sortings] = {"Bubble Sort", "Selection Sort", "Heap Sort", "Selection Sort"};

    kku::edit_arr_texts(kku::Kinds_sortings, names_of_sortings, names_sorts, &font);

    sf::RectangleShape sort_buttons[kku::Kinds_sortings];

    for (int i = 0; i < kku::Kinds_sortings; i++) {
        kku::edit_button(&sort_buttons[i]);
        sort_buttons[i].setPosition(20 + 200 * i, 100);
        names_of_sortings[i].setPosition(sort_buttons[i].getPosition().x + 10, sort_buttons[i].getPosition().y + 3);
    }

    sf::Text choose_fill;

    kku::edit_text(&choose_fill, "Choose the filling of array for sorting:", &font);
    choose_fill.setPosition(30, sort_buttons[0].getPosition().y + 50);

    sf::RectangleShape fill_button;

    kku::edit_button(&fill_button);
    fill_button.setPosition(20, choose_fill.getPosition().y + 50);

    sf::Text name_random_fill;

    kku::edit_text(&name_random_fill, "Random", &font);
    name_random_fill.setPosition(30, fill_button.getPosition().y + 3);

    sf::RectangleShape next_but;

    kku::edit_button(&next_but);
    next_but.setPosition(window.getSize().x - 150, window.getSize().y - 80);
    next_but.setSize(sf::Vector2f(100, 30));

    sf::Text next;

    kku::edit_text(&next, "Analys", &font);
    next.setPosition(next_but.getPosition().x + 10, next_but.getPosition().y + 3);

    bool ready[2] = {};
    int choose[2] = {};
    bool point = false;
    bool create_graphs = false;

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        pointer.setPosition(sf::Mouse::getPosition(window).x - 7, sf::Mouse::getPosition(window).y - 4);
        point = false;

        for (int i = 0; i < kku::Kinds_sortings; i++) {
            if (kku::check_border(sort_buttons[i], &window)) {
                window.setMouseCursorVisible(false);
                point = true;
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 200) {
                    clock.restart();
                    for (int j = 0; j < kku::Kinds_sortings; j ++) {
                        kku::is_active(&sort_buttons[j], (i == j && choose[0] != i));
                    }
                    ready[0] = choose[0] != i;
                    choose[0] = (choose[0] != i) ? i : -1;
                }
            }
        }
        if (kku::check_border(fill_button, &window)) {
            window.setMouseCursorVisible(false);
            point = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 200) {
                clock.restart();
                kku::is_active(&fill_button, !ready[1]);
                ready[1] = !ready[1];
            }
        }

        if (kku::check_border(next_but, &window)) {
            window.setMouseCursorVisible(false);
            point = true;
        }

        if (!point) {
            window.setMouseCursorVisible(true);
        }

        window.clear(sf::Color(230, 230, 230));
        window.setView(fixed);
        window.draw(brief);
        window.draw(choose_sort);
        for (int i = 0; i < kku::Kinds_sortings; i++) {
            window.draw(sort_buttons[i]);
            window.draw(names_of_sortings[i]);
        }
        window.draw(choose_fill);
        window.draw(fill_button);
        window.draw(name_random_fill);
        window.draw(next_but);
        window.draw(next);
        if (point)
            window.draw(pointer);
        window.display();
    }

    return 0;
}