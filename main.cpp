#include <SFML/Graphics.hpp>
#include <assert.h>
#include <cmath>
#include <stdio.h>

const int Kinds_sortings = 4;
const int Kinds_fillings = 1;

const int BUBBLE = 0, SELECTION = 1, HEAP = 2, MERGE = 3;

const sf::Color BACKGROUND_COL = sf::Color(230, 230, 230);
const sf::Color INACT_BUT_CIRCUIT = sf::Color(140, 140, 140);
//const sf::Color ACT_BUT_CIRCUIT = sf::Color::Green;
const sf::Color BUT_FILL = sf::Color(245, 245, 245);
const int OUTLINE_SIZE = 3;
const int THIN_OUTLINE_SIZE = 2;

const sf::Color TEXT_COL = sf::Color::Black;
const sf::Color GRAPH_TEXT_COL = sf::Color::White;
const sf::Color WARNING_TEXT_COL = sf::Color::Red;
const int TEXT_SIZE = 18;
const int WARNING_TEXT_SIZE = 20;
const int NUM_SIZE = 16;

const std::string BUBBLE_S = "Bubble sort",
                  SELECTION_S = "Selection sort",
                  HEAP_S = "Heap sort",
                  MERGE_S = "Merge Sort";

const std::string RAND_F = "Random";

const sf::Color GRAPH_BACK = sf::Color::Black;
const sf::Color FUNC_GRAPH_COL[Kinds_sortings] = {sf::Color::Green,
                                              sf::Color::Red,
                                              sf::Color::Blue,
                                              sf::Color(255, 102, 0)};

const int NMAX = 1500;

struct Size {
    int x, y;
};

const Size BUT_SIZE = {160, 30};

struct Text {
    int x, y;
    std::string write;
    sf::Font font;
    sf::Color color;
    int char_size;

    int angle = 0;
    void draw_text(sf::RenderWindow *window);
};

struct Button {
    int x, y;
    Size size;
    sf::Color fill;

    int out_size;
    std::string text;

    sf::Font font;

    int number;
    bool is_active = false;

    sf::Color out = (is_active) ? FUNC_GRAPH_COL[number] : INACT_BUT_CIRCUIT;

    Text fill_text = {x + 10, y + 3, text, font, TEXT_COL, TEXT_SIZE};
    sf::Vector2f vect_size = sf::Vector2f(size.x, size.y);

    void draw_button(sf::RenderWindow *window);
};

//struct Cursor {
//    sf::Sprite form;
//    sf::Texture texture;
//
//    bool is_pointer = false;
//
//    void show(sf::RenderWindow* window);
//};

struct Graph {
    sf::Vector2f left_top;

    std::string title,
                xlab,
                ylab;

    sf::Font font;

    int number;

    sf::Vector2f points[Kinds_sortings][NMAX];

    void draw_graph(sf::RenderWindow* window, const bool choose[]);
    void build_func_graphs(const bool* choose);
};

void copy_array(int ar1[], int ar2[], int n) {
    assert(0 < n && n <= NMAX);

    for (int i = 0; i < n; i++) {
        ar1[i] = ar2[i];
    }
}

void rand_array(int nmax, int array[]) {
    assert(0 < nmax && nmax <= NMAX);

    for (int i = 0; i < nmax; i++) {
        array[i] = rand();
    }
}

void bubble_sort(int array[], int permutations[], int comparisons[], int num_elem) {
    assert(0 < num_elem && num_elem <= NMAX);

    int t = -1;
    for (int i = 0; i < num_elem; i++) {
        for (int j = i + 1; j < num_elem; j++) {
            comparisons[num_elem]++;
            if (array[j] < array[i]) {
                t = array[j];
                array[j] = array[i];
                array[i] = t;
                permutations[num_elem]++;
            }
        }
    }
}

void selection_sort(int array[], int permutations[], int comparisons[], int num_elem) {
    assert(0 < num_elem && num_elem <= NMAX);

    int mn = array[0], ind_mn = 0;
    for (int i = 0; i < num_elem; i++) {
        for (int j = i + 1; j < num_elem; j++) {
            comparisons[num_elem]++;
            if (array[j] < mn) {
                mn = array[j];
                ind_mn = j;
            }
        }
        array[ind_mn] = array[i];
        array[i] = mn;
        permutations[num_elem]++;
    }

}

void heap_sort(int array[], int permutations[], int comparisons[], int num_elem) {

}

void merge_sort(int array[], int permutations[], int comparisons[], int num_elem) {

}

bool check_border(const Button& border, sf::RenderWindow *window) {
    return (border.x <= sf::Mouse::getPosition(*window).x
            && sf::Mouse::getPosition(*window).x <= border.x + border.size.x
            && border.y <= sf::Mouse::getPosition(*window).y
            && sf::Mouse::getPosition(*window).y <= border.y + border.size.y);
}

//std::string reverse_string(std::string str) {
//    std::string sec_str;
//    for (int i = str.length() - 1; i >= 0; i--) {
//        sec_str += str[i];
//    }
//    return sec_str;
//}

//std::string to_string(int n) {
//    if (!n)
//        return "0";
//    std::string n_str;
//    while (n > 0) {
//        n_str += (n % 10) + '0';
//        n /= 10;
//    }
//    return reverse_string(n_str);
//}

void Text::draw_text(sf::RenderWindow* window) {
    assert(typeid(write) == typeid(std::string));
    assert(x >= 0 && x < sf::VideoMode::getDesktopMode().width);
    assert(y >= 0 && y < sf::VideoMode::getDesktopMode().height);
    assert(char_size > 10 && char_size < 36);

    sf::Text text;

    text.setFont(font);
    text.setString(write);
    text.setFillColor(color);
    text.setCharacterSize(char_size);
    text.setPosition(x, y);
    text.rotate(angle);

    window->draw(text);
}

void Button::draw_button(sf::RenderWindow* window) {
    assert(x >= 0 && x < sf::VideoMode::getDesktopMode().width);
    assert(y >= 0 && y < sf::VideoMode::getDesktopMode().height);

    sf::RectangleShape shape;
    out = (is_active) ? FUNC_GRAPH_COL[number] : INACT_BUT_CIRCUIT;

    shape.setPosition(x, y);
    shape.setSize(vect_size);
    shape.setFillColor(fill);
    shape.setOutlineColor(out);
    shape.setOutlineThickness(out_size);

    window->draw(shape);
    fill_text.draw_text(window);
}

void Graph::draw_graph(sf::RenderWindow *window, const bool choose[]) {
    assert(left_top.x >= 0 && left_top.x < sf::VideoMode::getDesktopMode().width - 301);
    assert(left_top.y >= 0 && left_top.y < sf::VideoMode::getDesktopMode().height - 402);

    sf::RectangleShape back;
    back.setPosition(left_top);
    back.setSize(sf::Vector2f(301, 402));

    Text zero = {static_cast<int>(left_top.x - 12), static_cast<int>(left_top.y + 405), "0",
                 font, TEXT_COL, NUM_SIZE},
         xmax = {static_cast<int>(left_top.x + 290), static_cast<int>(left_top.y + 405), "1500",
                 font, TEXT_COL, NUM_SIZE},
         ymax = {static_cast<int>(left_top.x - ((number == 1) ? 40 : 65)), static_cast<int>(left_top.y), (number == 1) ? "4210" : "2000000",
                 font, TEXT_COL, NUM_SIZE},
         xmid = {static_cast<int>(left_top.x + 145), static_cast<int>(left_top.y + 405), "750",
                 font, TEXT_COL, NUM_SIZE},
         ymid = {static_cast<int>(left_top.x - ((number == 1) ? 40 : 65)), static_cast<int>(left_top.y + 200), (number == 1) ? "2105" : "1000000",
                 font, TEXT_COL, NUM_SIZE};

    back.setFillColor(GRAPH_BACK);

    Text title_text = {static_cast<int>(left_top.x - 50), static_cast<int>(left_top.y - 40), title, font,
                       TEXT_COL, TEXT_SIZE},
         xlab_text =  {static_cast<int>(left_top.x + 50), static_cast<int>(left_top.y + 420), xlab, font,
                       TEXT_COL, TEXT_SIZE},
         ylab_text =  {static_cast<int>(left_top.x - 90), static_cast<int>(left_top.y + 280), ylab, font,
                       TEXT_COL, TEXT_SIZE, -90};

    window->draw(back);
    title_text.draw_text(window);
    xlab_text.draw_text(window);
    ylab_text.draw_text(window);
    zero.draw_text(window);
    xmax.draw_text(window);
    ymax.draw_text(window);
    xmid.draw_text(window);
    ymid.draw_text(window);

    sf::CircleShape circleShape;

    for (int type = 0; type < Kinds_sortings; type ++) {
        for (auto point : points[type]) {
            if (!choose[type] || point.x >= left_top.x + 300 || point.y <= left_top.y) continue;
            circleShape.setRadius(1);
            circleShape.setPosition(point);
            circleShape.setFillColor(FUNC_GRAPH_COL[type]);
            window->draw(circleShape);
        }
    }
}

void draw_buttons(Button buttons[], int num_of_elems, sf::RenderWindow* window) {
    assert(0 < num_of_elems && num_of_elems <= NMAX);

    for (int i = 0; i < num_of_elems; i ++) {
        buttons[i].draw_button(window);
    }
}

bool is_clicked(sf::Clock clock) {
    return (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 200);
}

//void Cursor::show(sf::RenderWindow *window) {
//    window->setMouseCursorVisible(!is_pointer);
//}

void Graph::build_func_graphs(const bool choose[Kinds_sortings]) {
    int arr[NMAX] = {};
    rand_array(NMAX, arr);
    int copy_of_arr[NMAX];

    for (int type = 0; type < Kinds_sortings; type ++) {
        if (!choose[type]) continue;
        int coms[NMAX] = {};
        int perms[NMAX] = {};

        for (int n = 0; n < NMAX; n ++) {
            copy_array(copy_of_arr, arr, n);
            switch (type) {
                case BUBBLE:
                    bubble_sort(copy_of_arr, perms, coms, n);
                    break;
                case SELECTION:
                    selection_sort(copy_of_arr, perms, coms, n);
                    break;
                case HEAP:
                    heap_sort(copy_of_arr, perms, coms, n);
                    break;
                case MERGE:
                    merge_sort(copy_of_arr, perms, coms, n);
                    break;
                default:
                    break;
            }
        }
        for (int i = 0; i < NMAX; i ++) {
            points[type][i] = sf::Vector2f(i * 0.1992 + ((number == 1) ? 300 : 820),
                    650 - ((number == 1) ? perms[i] * 0.095 : coms[i] * 0.0002));

            if (i < 120) printf("%i sorting, %i num: %i, %i \n", type, i, perms[i], coms[i]);
        }

    }

}

void build_app(sf::RenderWindow* window, sf::RenderWindow* warning_window, Button sort_buttons[], Button fill_buttons[], Button* next_but,
        int ready[], bool choose[][std::max(Kinds_fillings, Kinds_sortings)],
        Text* brief, Text* choose_sort, Text* choose_fill, Text* warning, bool* is_warning, sf::View fixed,
        Graph* graph1, Graph* graph2) {
    sf::Clock clock;
    bool is_necessary = true;

    while (window->isOpen()) {
        sf::Event event{};
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape))
                window->close();
        }

        while (warning_window->pollEvent(event)) {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)) {
                is_necessary = false;
                warning_window->close();
            }
        }

        //cursor->form.setPosition(sf::Mouse::getPosition(*window).x - 7, sf::Mouse::getPosition(*window).y - 4);
        //cursor->is_pointer = false;

        for (int i = 0; i < Kinds_sortings; i++) {
            if (check_border(sort_buttons[i], window)) {
                //cursor->is_pointer = true;
                if (is_clicked(clock)) {
                    clock.restart();
                    choose[0][i] = (!choose[0][i]);
                    ready[0] += (choose[0][i]) ? 1 : -1;
                    sort_buttons[i].is_active = choose[0][i];
                }
            }
        }
        if (check_border(fill_buttons[0], window)) {
//            cursor->is_pointer = true;
            if (is_clicked(clock)) {
                clock.restart();
                ready[1] = 1 - ready[1];
                fill_buttons[0].is_active = ready[1];
            }
        }

        if (check_border(*next_but, window)) {
//            cursor->is_pointer = true;
            if (is_clicked(clock)) {
                clock.restart();
                if (ready[0] && ready[1]) {
                    *is_warning = false;
                    graph1->build_func_graphs(choose[0]);
                    graph2->build_func_graphs(choose[0]);
                } else {
                    *is_warning = true;
                    is_necessary = true;
                    std::string warn;
                    if (ready[0]) warn = "\tYou didn't chose the kind of filling arrays";
                    else if (ready[1]) warn = "\tYou didn't chose the kind of sortings";
                    else warn = "You didn't chose the kinds of sortings and filling arrays";
                    warning->write = warn;
                }
            }
        }

//        cursor->show(window);


        window->clear(BACKGROUND_COL);
        window->setView(fixed);

        brief->draw_text(window);
        choose_sort->draw_text(window);

        draw_buttons(sort_buttons, Kinds_sortings, window);

        choose_fill->draw_text(window);

        draw_buttons(fill_buttons, Kinds_fillings, window);

        next_but->draw_button(window);

        graph1->draw_graph(window, choose[0]);
        graph2->draw_graph(window, choose[0]);

//        if (cursor->is_pointer)
//            window->draw(cursor->form);
        window->display();

        if (*is_warning)
        {
            if (!warning_window->isOpen() && is_necessary) warning_window->create(sf::VideoMode(600, 100), "Warning!");
            warning_window->clear(sf::Color::White);
            warning->draw_text(warning_window);
            warning_window->display();
        }
    }

}

int main() {

    freopen("../output.out", "w", stdout);

    sf::RenderWindow window(
            sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height),
            "Analysis sortings");
    sf::RenderWindow warning_window;

    sf::View fixed = window.getView();
//    Cursor cursor;
//    cursor.texture.loadFromFile("../pointer.png");
//    cursor.form.setTexture(cursor.texture);

    sf::Font font;

    if (!font.loadFromFile("../Roboto-Regular.ttf"))
        return 1;

    Text brief = {20, 35, "There's you can analys sortings. Choose the kind of sortings and the kind of fillings.",
                  font, TEXT_COL, TEXT_SIZE};

    Text choose_sort = {20, 65, "Kinds of sortings:", font, TEXT_COL, TEXT_SIZE};

    Button sort_buttons[4] = {{ 30, 100, BUT_SIZE, BUT_FILL, OUTLINE_SIZE, BUBBLE_S,    font, BUBBLE},
                              {230, 100, BUT_SIZE, BUT_FILL, OUTLINE_SIZE, SELECTION_S, font, SELECTION},
                              {430, 100, BUT_SIZE, BUT_FILL, OUTLINE_SIZE, HEAP_S,      font, HEAP},
                              {630, 100, BUT_SIZE, BUT_FILL, OUTLINE_SIZE, MERGE_S,     font, MERGE}};

    Text choose_fill = {20, 150, "Choose the filling of array for sorting:", font, TEXT_COL, TEXT_SIZE};

    Button fill_buttons[1] = {{30, 200, BUT_SIZE, BUT_FILL, OUTLINE_SIZE, RAND_F, font, 0}};

    Button next_but = {static_cast<int>(sf::VideoMode::getDesktopMode().width - 200),
                       static_cast<int>(sf::VideoMode::getDesktopMode().height - 120),
                       BUT_SIZE, BUT_FILL, OUTLINE_SIZE, "Start", font, 0};

    Text warning = {50, 40, "", font, WARNING_TEXT_COL, WARNING_TEXT_SIZE};

    int ready[2] = {};
    bool choose[2][std::max(Kinds_sortings, Kinds_fillings)] = {};
    bool is_warning = false;
    sf::CircleShape circleShapes[Kinds_sortings][NMAX];

    for (auto & circleShape : circleShapes) {
        for (auto & i : circleShape) {
            i = sf::CircleShape(10);
        }
    }

    Graph graph1 = {sf::Vector2f(300, 250),
                    "Dependence of the number of permutations from N", "Num of array's elements", "Num of permutations",
                    font, 1};

    Graph graph2 = {sf::Vector2f(820, 250),
                    "Dependence of the number of comparisons from N", "Num of array's elements", "Num of comparisons",
                    font, 2};


    build_app(&window, &warning_window, sort_buttons, fill_buttons, &next_but, ready, choose, &brief, &choose_sort,
            &choose_fill, &warning, &is_warning, fixed, &graph1, &graph2);

    return 0;
}