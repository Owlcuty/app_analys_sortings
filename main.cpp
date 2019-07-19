#define BUBBLE 0
#define SELECTION 1
#define HEAP 2
#define MERGE 3

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <cmath>

namespace kku {

    const int Kinds_sortings = 4;
    const int Kinds_fillings = 1;
    const int Start_x = 50;
    const int Start_y = 460;
    const int Finish_x = 470;
    const int Finish_y = 30;

    void copy_array(int ar1[], int ar2[], int n) {
        for (int i = 0; i < n; i ++) {
            ar1[i] = ar2[i];
        }
    }

    void rand_array(int nmax, int array[]) {
        for (int i = 0; i < nmax; i ++) {
            array[i] = rand();
        }
    }

    void bubble_sort(int array[], int permutations[], int comparisons[], int num_elem) {
        int t = -1;
        for (int i = 0; i < num_elem; i ++) {
            for (int j = i + 1; j < num_elem; j ++) {
                comparisons[num_elem] ++;
                if (array[j] < array[i]) {
                    t = array[j];
                    array[j] = array[i];
                    array[i] = t;
                    permutations[num_elem] ++;
                }
            }
        }
    }
    void selection_sort(int array[], int permutations[], int comparisons[], int num_elem) {
        int mn = array[0], ind_mn = 0;
        for (int i = 0; i < num_elem; i ++) {
            for (int j = i + 1; j < num_elem; j ++) {
                comparisons[num_elem] ++;
                if (array[j] < mn) {
                    mn = array[j];
                    ind_mn = j;
                }
            }
            array[ind_mn] = array[i];
            array[i] = mn;
            permutations[num_elem] ++;
        }
    }
    void heap_sort(int array[], int permutations[], int comparisons[], int num_elem) {

    }
    void merge_sort(int array[], int permutations[], int comparisons[], int num_elem) {

    }


    void is_active(sf::RectangleShape* button, bool active) {
        button->setOutlineColor((active) ? sf::Color::Green : sf::Color(140, 140, 140));
    }


    bool check_border(sf::RectangleShape border, sf::RenderWindow* window) {
        return (border.getPosition().x <= sf::Mouse::getPosition(*window).x
                && sf::Mouse::getPosition(*window).x <= border.getPosition().x + border.getSize().x
                && border.getPosition().y <= sf::Mouse::getPosition(*window).y
                && sf::Mouse::getPosition(*window).y <= border.getPosition().y + border.getSize().y);
    }

    void edit_text(sf::Text* text, std::string write, sf::Font* font) {
        text->setFont(*font);
        text->setString(write);
        text->setColor(sf::Color::Black);
        text->setCharacterSize(18);
    }

    void edit_warning_text(sf::Text* text, std::string write, sf::Font *font) {
        text->setFont(*font);
        text->setString(write);
        text->setColor(sf::Color::Red);
        text->setCharacterSize(22);
    }

    void edit_arr_texts(int cnt_of_elements, sf::Text text[], std::string write[], sf::Font* font) {
        for (int i = 0; i < cnt_of_elements; i++) {
            edit_text(&text[i], write[i], font);
        }
    }

    void edit_button(sf::RectangleShape* but) {
        but->setFillColor(sf::Color(245, 245, 245));
        but->setOutlineThickness(3);
        but->setOutlineColor(sf::Color(140, 140, 140));
        but->setSize(sf::Vector2f(160, 30));
    }

    void build_counter(sf::RectangleShape counter[]) {
        for (int i = 0; i < 3; i ++) {
            counter[i].setFillColor(sf::Color(240, 240, 240));
            counter[i].setOutlineThickness(2);
            counter[i].setOutlineColor(sf::Color(140, 140, 140));
            counter[i].setSize(sf::Vector2f(25, 30));
        }
        counter[1].setSize(sf::Vector2f(200, 30));
        counter[1].setOutlineColor(sf::Color(140, 140, 140));
    }

    std::string reverse_string(std::string str) {
        std::string sec_str = "";
        for (int i = str.length() - 1; i >= 0; i --) {
            sec_str += str[i];
        }
        return sec_str;
    }

    std::string to_string(int n) {
        if (!n)
            return "0";
        std::string n_str = "";
        while (n > 0) {
            n_str += (n % 10) + '0';
            n /= 10;
        }
        return reverse_string(n_str);
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
    std::string names_sorts[kku::Kinds_sortings] = {"Bubble Sort", "Selection Sort", "Heap Sort", "Merge Sort"};

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

    sf::Text input_nmax;

    kku::edit_text(&input_nmax, "Max number of array's size:", &font);
    input_nmax.setPosition(30, fill_button.getPosition().y + 50);

    sf::RectangleShape cnt_nmax[3];

    kku::build_counter(cnt_nmax);
    for (int i = 0; i < 3; i ++) {
        cnt_nmax[i].setPosition(i ? cnt_nmax[i - 1].getPosition().x + cnt_nmax[i - 1].getSize().x + 10 : 20,
                input_nmax.getPosition().y + 50);
    }

    int nmax = 0;
    sf::Text text_cnt_nmax[3];
    std::string str_cnt_max[3] = {"-", kku::to_string(nmax), "+"};

    kku::edit_arr_texts(3, text_cnt_nmax, str_cnt_max, &font);
    for (int i = 0; i < 3; i += 2) {
        text_cnt_nmax[i].setPosition(cnt_nmax[i].getPosition().x + 8, cnt_nmax[i].getPosition().y + 3);
    }
    text_cnt_nmax[1].setPosition(cnt_nmax[1].getPosition().x
                              + (cnt_nmax[1].getPosition().x + cnt_nmax[1].getSize().x) / 2 - 8 * str_cnt_max[1].length(),
                                 cnt_nmax[1].getPosition().y + 3);


    sf::RectangleShape next_but;

    kku::edit_button(&next_but);
    next_but.setPosition(window.getSize().x - 150, window.getSize().y - 80);
    next_but.setSize(sf::Vector2f(100, 30));

    sf::Text next;

    kku::edit_text(&next, "Analys", &font);
    next.setPosition(next_but.getPosition().x + 10, next_but.getPosition().y + 3);

    sf::Text warning;
    warning.setPosition(30, window.getSize().y - 80);

    bool ready[2] = {};
    int choose[2] = {};
    bool point = false;
    bool create_graphs = false;
    bool is_warning = false;
    int permutations[100000] = {}, comparisons[100000] = {};
    int array_for_sort[100000] = {};
    int array_for_copy[100000] = {};

    sf::Vertex xy[4] = {
            sf::Vertex(sf::Vector2f(kku::Start_x, kku::Finish_y)),
            sf::Vertex(sf::Vector2f(kku::Start_x, kku::Start_y)),
            sf::Vertex(sf::Vector2f(kku::Start_x, kku::Start_y)),
            sf::Vertex(sf::Vector2f(kku::Finish_x, kku::Start_y))
    };

    sf::Vertex perms[100000] = {};
    sf::Vertex comps[100000] = {};

    perms[0] = sf::Vertex(sf::Vector2f(kku::Start_x, kku::Start_y));
    comps[0] = sf::Vertex(sf::Vector2f(kku::Start_x, kku::Start_y));

    sf::Text x_brief, y1_brief, y2_brief;

    kku::edit_text(&x_brief, "Number of elements", &font);
    kku::edit_text(&y1_brief, "Number of permutaions", &font);
    kku::edit_text(&y2_brief, "Number of comparisons", &font);
    x_brief.setColor(sf::Color::White);
    y1_brief.setColor(sf::Color::White);
    y2_brief.setColor(sf::Color::White);
    x_brief.setPosition(150, kku::Start_y + 10);
    y1_brief.setPosition(kku::Start_x - 30, kku::Start_y - 130);
    y1_brief.rotate(-90);
    y2_brief.setPosition(kku::Start_x - 30, kku::Start_y - 130);
    y2_brief.rotate(-90);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                window.close();
        }
        while (graph1.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                graph1.close();
        }
        while (graph2.pollEvent(event)) {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                graph2.close();
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 300) {
                clock.restart();
                if (ready[0] && ready[1]) {
                    is_warning = false;
                    create_graphs = true;
                } else {
                    is_warning = true;
                    std::string warn = "";
                    if (ready[0]) warn = "You didn't chose the kind of filling arrays";
                    else if (ready[1]) warn = "You didn't chose the kind of sortings";
                    else warn = "You didn't chose the kinds of sortings and filling arrays";
                    kku::edit_warning_text(&warning, warn, &font);
                }
            }
        }


        if (kku::check_border(cnt_nmax[0], &window)) {
            window.setMouseCursorVisible(false);
            point = true;
            if (nmax > 0 && sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 100) {
                clock.restart();
                nmax --;
                kku::edit_text(&text_cnt_nmax[1], kku::to_string(nmax), &font);
            }
        }

        if (kku::check_border(cnt_nmax[2], &window)) {
            window.setMouseCursorVisible(false);
            point = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 100) {
                clock.restart();
                nmax ++;
                kku::edit_text(&text_cnt_nmax[1], kku::to_string(nmax), &font);
            }
        }


        if (!point) {
            window.setMouseCursorVisible(true);
        }

        if (create_graphs) {
            graph1.create(sf::VideoMode(500, 500), "Dependence of the number of permutations from N");
            graph2.create(sf::VideoMode(500, 500), "Dependence of the number of comparisons from N");
            graph2.setPosition(sf::Vector2i(graph1.getPosition().x + 600, graph1.getPosition().y));
            create_graphs = false;

            kku::rand_array(nmax, array_for_sort);
            for (int n = 1; n < nmax; n ++) {
                kku::copy_array(array_for_copy, array_for_sort, n);
                switch (choose[0]) {
                    case BUBBLE:
                        kku::bubble_sort(array_for_copy, permutations, comparisons, n);
                        perms[n * 2 - 1] = sf::Vertex(sf::Vector2f(kku::Start_x + n * 10, kku::Start_y - permutations[n] * 8));
                        perms[n * 2] = perms[n * 2 - 1];
                        comps[n * 2 - 1] = sf::Vertex(sf::Vector2f(kku::Start_x + n * 10, kku::Start_y - comparisons[n] * 8));
                        comps[n * 2] = comps[n * 2 - 1];
                        break;
                    case SELECTION:
                        kku::selection_sort(array_for_copy, permutations, comparisons, n);
                        perms[n * 2 - 1] = sf::Vertex(sf::Vector2f(kku::Start_x + n * 10, kku::Start_y - permutations[n] * 8));
                        perms[n * 2] = perms[n * 2 - 1];
                        comps[n * 2 - 1] = sf::Vertex(sf::Vector2f(kku::Start_x + n * 10, kku::Start_y - comparisons[n] * 8));
                        comps[n * 2] = comps[n * 2 - 1];
                        break;
                    case HEAP:
                        kku::heap_sort(array_for_copy, permutations, comparisons, n);
                        break;
                    case MERGE:
                        kku::merge_sort(array_for_copy, permutations, comparisons, n);
                        break;
                    default:
                        kku::edit_warning_text(&warning, "choose[0] is not 0-3", &font);
                        is_warning = true;
                        break;
                }
            }


        }

        window.clear(sf::Color(230, 230, 230));
        graph1.clear();
        graph2.clear();
//        graph1.clear(sf::Color(240, 240, 240));
//        graph2.clear(sf::Color(240, 240, 240));
        window.setView(fixed);
        window.draw(brief);
        window.draw(choose_sort);
        for (int i = 0; i < kku::Kinds_sortings; i++) {
            window.draw(sort_buttons[i]);
            window.draw(names_of_sortings[i]);
        }
        window.draw(input_nmax);
        for (int i = 0; i < 3; i ++) {
            window.draw(cnt_nmax[i]);
            window.draw(text_cnt_nmax[i]);
        }
        window.draw(choose_fill);
        window.draw(fill_button);
        window.draw(name_random_fill);
        window.draw(next_but);
        window.draw(next);
        graph1.draw(xy, 4, sf::Lines);
        graph2.draw(xy, 4, sf::Lines);
        graph1.draw(perms, nmax, sf::Lines);
        graph1.draw(x_brief);
        graph2.draw(x_brief);
        graph1.draw(y1_brief);
        graph2.draw(y2_brief);
        graph2.draw(comps, nmax, sf::Lines);
        if (is_warning)
            window.draw(warning);
        if (point)
            window.draw(pointer);
        window.display();
        graph1.display();
        graph2.display();
    }

    return 0;
}