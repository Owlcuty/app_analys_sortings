#include <stdio.h>
#include <assert.h>
#include <cmath>

#include <SFML/Graphics.hpp>
#include <cstring>

enum Types_sorting {
    Sort_empty = 0, Sort_bubble = 1, Sort_selection = 2, Sort_heap = 3, Sort_insertion = 4, Num_types_sortings = 5
};
enum Types_filling {
    Fill_empty = 0, Fill_random = 1, Num_types_fillings = 2
};

typedef sf::Vector2f Size;
typedef sf::Vector2f Coord;

namespace Settings {
    const sf::Color Background_color = sf::Color(230, 230, 230);
    const sf::Color Inact_btn_color = sf::Color(140, 140, 140);
//const sf::Color ACT_BUT_CIRCUIT = sf::Color::Green;
    const sf::Color Btn_fill = sf::Color(245, 245, 245);
    const int Outline_size = 3;
    const int Thin_outline_size = 2;

    const int Height = 768;
    const int Width = 1366;

    const sf::Color Text_col = sf::Color::Black;
    const sf::Color Graph_text_color = sf::Color::White;
    const sf::Color Warning_text_color = sf::Color::Red;
    const int Text_size = 18;
    const int Warning_text_size = 20;
    const int Num_size = 16;

    const std::string Bubble_str = "Bubble sort",
            Selection_str = "Selection sort",
            Heap_str = "Heap sort",
            Insertion_str = "Insertion Sort";

    const std::string Rand_fill_str = "Random";

    const sf::Color Graph_backgrnd = sf::Color::Black;
    const sf::Color Func_graph_color[Num_types_sortings] = {Inact_btn_color,
                                                            sf::Color::Green,
                                                            sf::Color::Red,
                                                            sf::Color::Blue,
                                                            sf::Color(255, 102, 0)};
    const sf::Color Fill_act_col[Num_types_fillings] = {Inact_btn_color,
                                                        sf::Color::Black};

    const int NMax_for_sort = 1500;
    const int NMax_for_arrays = 100000;

    const Size Btn_size = {160, 30};

}

int* copy_of_arr = static_cast<int *>(calloc(Settings::NMax_for_sort, sizeof(int)));
int* coms = static_cast<int *>(calloc(Settings::NMax_for_sort, sizeof(*coms)));
int* perms = static_cast<int *>(calloc(Settings::NMax_for_sort, sizeof(*perms)));

// ----- Text ----------------------------------------------------------------------------------------------------------

struct Text {
    Coord point;
    std::string str;
    sf::Font font;
    sf::Color color;
    int char_size;

    int angle = 0;

    void draw(sf::RenderWindow *window);
};


void Text::draw(sf::RenderWindow *window) {
    assert(0 <= point.x && point.x < Settings::Width);
    assert(0 <= point.y && point.y < Settings::Height);
    assert(char_size > 10 && char_size < 36);

    sf::Text text;

    text.setFont(font);
    text.setString(str);
    text.setFillColor(color);
    text.setCharacterSize(char_size);
    text.setPosition(point);
    text.rotate(angle);

    window->draw(text);
}

// ---------------------------------------------------------------------------------------------------------------------

//class Manager_btns {
//
//};


// ----- Button's engine -----------------------------------------------------------------------------------------------

struct Button {

    void (*func)(int array[], int permutations[], int comparisons[], int num_elem);

    std::string text;

    int id;

    bool is_active = false;

    Coord point;
    int number;

    sf::Font font;

    Size size = Settings::Btn_size;
    sf::Color fill_col = Settings::Btn_fill;

    int outline_size = Settings::Outline_size;


    //-----------------------------

    sf::Color outline_color;

    Text fill_text;

    //-----------------------------

    void draw(sf::RenderWindow *window);
};

void Button::draw(sf::RenderWindow *window) {
    assert(0 <= point.x && point.x < Settings::Width);
    assert(0 <= point.y && point.y < Settings::Height);

    outline_color = Settings::Func_graph_color[(int) (is_active) * number];
    if (id == 6) {
        outline_color = Settings::Fill_act_col[(int) (is_active) * number];
    }
    fill_text = {{point.x + 10, point.y + 3}, text, font, Settings::Text_col, Settings::Text_size};

    sf::RectangleShape shape;

    shape.setPosition(point);
    shape.setSize(size);
    shape.setFillColor(fill_col);
    shape.setOutlineColor(outline_color);
    shape.setOutlineThickness(outline_size);

    window->draw(shape);
    fill_text.draw(window);

    printf("%i: %i\n", number, is_active);
}

void draw_buttons(Button buttons[], int num_of_elems, sf::RenderWindow *window) {
    assert(0 < num_of_elems && num_of_elems <= Settings::NMax_for_arrays);

    for (int i = 1; i < num_of_elems; i++) {
        buttons[i].draw(window);
    }
}

void set_buttons(Button buttons[], int num_of_elems, int step_x, int step_y) {
    sf::Font font;
    sf::Color fill_col;
    Coord start_pos;
    int outline_size;

    for (int num = 0; num < num_of_elems; num++) {
        if (!num) {
            font = buttons[0].font;
            fill_col = buttons[num].fill_col;
            start_pos = buttons[num].point;
            outline_size = buttons[num].outline_size;
            buttons[num].point = {0, 0};
            buttons[num].size = {0, 0};
            continue;
        }
        buttons[num].point = {start_pos.x + step_x * (num - 1), start_pos.y + step_y * (num - 1)};
        buttons[num].font = font;
        buttons[num].fill_col = fill_col;
        buttons[num].outline_size = outline_size;
        buttons[num].number = num;
    }
}

// ---------------------------------------------------------------------------------------------------------------------


//struct Cursor {
//    sf::Sprite form;
//    sf::Texture texture;
//
//    bool is_pointer = false;
//
//    void show(sf::RenderWindow* window);
//};


// ----- Graph ---------------------------------------------------------------------------------------------------------

struct Graph {
    Coord left_top;

    std::string title,
            xlab,
            ylab;

    sf::Font font;

    int number;

    sf::Vector2f points[Num_types_sortings][Settings::NMax_for_sort];

    void draw_graph(sf::RenderWindow *window, const bool choose[]);

    void build_func_graphs(const bool *choose, Button buttons[]);
};

// ---------------------------------------------------------------------------------------------------------------------

void rand_array(int nmax, int array[]);


void copy_array(int ar1[], int ar2[], int n) {
    assert(0 <= n && n <= Settings::NMax_for_arrays);

    for (int i = 0; i < n; i++) {
        ar1[i] = ar2[i];
    }
}

void rand_array(int nmax, int array[]) {
    assert(0 <= nmax && nmax <= Settings::NMax_for_arrays);

    for (int i = 0; i < nmax; i++) {
        array[i] = rand();
    }
}

void empty_sort(int array[], int permutations[], int comparisons[], int num_elem) {

}

void bubble_sort(int array[], int permutations[], int comparisons[], int num_elem) {
    assert(0 <= num_elem && num_elem <= Settings::NMax_for_arrays);

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
    assert(0 <= num_elem && num_elem <= Settings::NMax_for_arrays);

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

void insertion_sort(int array[], int *permutations, int *comparisons, int num_elem) {
    int key = 0;
    int i = 0;
    for (int j = 1; j < num_elem; j++) {
        key = array[j];
        i = j - 1;
        if (!(i >= 0 && array[i] > key)) comparisons[num_elem] ++;
        while (i >= 0 && array[i] > key) {
            array[i + 1] = array[i];
            i = i - 1;
            array[i + 1] = key;
            comparisons[num_elem] ++;
            permutations[num_elem] ++;
        }
    }
}

bool check_border(const Button &border, sf::RenderWindow *window) {
    return (                      border.point.x <= sf::Mouse::getPosition(*window).x
            && sf::Mouse::getPosition(*window).x <= border.point.x   +  border.size.x
                               && border.point.y <= sf::Mouse::getPosition(*window).y
            && sf::Mouse::getPosition(*window).y <= border.point.y   +  border.size.y);
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


void Graph::draw_graph(sf::RenderWindow *window, const bool choose[]) {
    assert(left_top.x >= 0 && left_top.x < Settings::Width - 301);
    assert(left_top.y >= 0 && left_top.y < Settings::Height - 402);

    sf::RectangleShape back;
    back.setPosition(left_top);
    back.setSize(sf::Vector2f(301, 402));

    Text zero = {{left_top.x - 12, left_top.y + 405}, "0", font, Settings::Text_col, Settings::Num_size},
            xmax = {{left_top.x + 290, left_top.y + 405}, "1500", font, Settings::Text_col, Settings::Num_size},
            ymax = {{left_top.x - ((number == 1) ? 40 : 65), left_top.y}, (number == 1) ? "4210" : "2000000",
                    font, Settings::Text_col, Settings::Text_size},
            xmid = {{left_top.x + 145, left_top.y + 405}, "750", font, Settings::Text_col, Settings::Num_size},
            ymid = {{left_top.x - ((number == 1) ? 40 : 65), left_top.y + 200}, (number == 1) ? "2105" : "1000000",
                    font, Settings::Text_col, Settings::Text_size};

    back.setFillColor(Settings::Graph_backgrnd);

    Text title_text = {{left_top.x - 50, left_top.y - 40}, title, font, Settings::Text_col, Settings::Text_size},
            xlab_text = {{left_top.x + 50, left_top.y + 420}, xlab, font, Settings::Text_col, Settings::Text_size},
            ylab_text = {{left_top.x - 90, left_top.y + 280}, ylab, font, Settings::Text_col, Settings::Text_size, -90};

    window->draw(back);
    title_text.draw(window);
    xlab_text.draw(window);
    ylab_text.draw(window);
    zero.draw(window);
    xmax.draw(window);
    ymax.draw(window);
    xmid.draw(window);
    ymid.draw(window);

    sf::CircleShape circleShape;

    for (int type = 0; type < Num_types_sortings; type++) {
        for (auto point : points[type]) {
            if (!choose[type] || point.x >= left_top.x + 300 || point.y <= left_top.y) continue;
            circleShape.setRadius(1);
            circleShape.setPosition(point);
            circleShape.setFillColor(Settings::Func_graph_color[type]);
            window->draw(circleShape);
        }
    }
}

bool is_clicked(sf::Clock clock) {
    return (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asMilliseconds() > 200);
}

//void Cursor::show(sf::RenderWindow *window) {
//    window->setMouseCursorVisible(!is_pointer);
//}

void Graph::build_func_graphs(const bool choose[Num_types_sortings], Button buttons[]) {
    int arr[Settings::NMax_for_sort] = {};
    rand_array(Settings::NMax_for_sort, arr);

//    int copy_of_arr[Settings::NMax_for_sort] = {};

    for (int type = 0; type < Num_types_sortings; type++) {
        if (!choose[type]) continue;
        memset(coms, 0, sizeof(*coms) * Settings::NMax_for_sort);
        memset(perms, 0, sizeof(*coms) * Settings::NMax_for_sort);

        for (int n = 0; n < Settings::NMax_for_sort; n++) {
            copy_array(copy_of_arr, arr, n);
            buttons[type].func(copy_of_arr, perms, coms, n);
        }
        for (int i = 0; i < Settings::NMax_for_sort; i++) {
            points[type][i] = sf::Vector2f(i * 0.1992 + ((number == 1) ? 300 : 820),
                                           650 - ((number == 1) ? perms[i] * 0.095 : coms[i] * 0.0002));

//            if (i < 120) printf("%i sorting, %i num: %i, %i \n", type, i, perms[i], coms[i]);
        }

    }
}

void destroy_from_building_funcs() {
    free(copy_of_arr);
    free(coms);
    free(perms);

}

void build_app(sf::RenderWindow *window, sf::RenderWindow *warning_window, Button sort_buttons[], Button fill_buttons[],
               Button *next_but,
               int ready[], bool choose[][Num_types_sortings],
               Text *brief, Text *choose_sort, Text *choose_fill, Text *warning, bool *is_warning, sf::View fixed,
               Graph *graph1, Graph *graph2) {
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

        for (int i = 0; i < Num_types_sortings; i++) {
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
        for (int i = 0; i < Num_types_fillings; i ++) {
            if (check_border(fill_buttons[i], window)) {
//            cursor->is_pointer = true;
                if (is_clicked(clock)) {
                    clock.restart();
                    ready[1] = 1 - ready[1];
                    fill_buttons[i].is_active = ready[1];
                }
            }
        }

        if (check_border(*next_but, window)) {
//            cursor->is_pointer = true;
            if (is_clicked(clock)) {
                clock.restart();
                if (ready[0] && ready[1]) {
                    *is_warning = false;
                    graph1->build_func_graphs(choose[0], sort_buttons);
                    graph2->build_func_graphs(choose[0], sort_buttons);
                } else {
                    *is_warning = true;
                    is_necessary = true;
                    std::string warn;
                    if (ready[0]) warn = "\tYou didn't chose the kind of filling arrays";
                    else if (ready[1]) warn = "\tYou didn't chose the kind of sortings";
                    else warn = "You didn't chose the kinds of sortings and filling arrays";
                    warning->str = warn;
                }
            }
        }

//        cursor->show(window);


        window->clear(Settings::Background_color);
        window->setView(fixed);

        brief->draw(window);
        choose_sort->draw(window);

        draw_buttons(sort_buttons, Num_types_sortings, window);

        choose_fill->draw(window);

        draw_buttons(fill_buttons, Num_types_fillings, window);

        next_but->draw(window);

        graph1->draw_graph(window, choose[0]);
        graph2->draw_graph(window, choose[0]);

//        if (cursor->is_pointer)
//            window->draw(cursor->form);
        window->display();

        if (*is_warning) {
            if (!warning_window->isOpen() && is_necessary) warning_window->create(sf::VideoMode(600, 100), "Warning!");
            warning_window->clear(sf::Color::White);
            warning->draw(warning_window);
            warning_window->display();
        }
    }

    destroy_from_building_funcs();

}

int main() {

    freopen("output.out", "w", stdout);

    sf::RenderWindow window(sf::VideoMode(Settings::Width, Settings::Height), "Analysis sortings");
    sf::RenderWindow warning_window;

    sf::View fixed = window.getView();
//    Cursor cursor;
//    cursor.texture.loadFromFile("../pointer.png");
//    cursor.form.setTexture(cursor.texture);

    sf::Font font;

    if (!font.loadFromFile("Roboto-Regular.ttf"))
        return 1;

    Text brief = {{20, 35}, "There's you can analys sortings. Choose the kind of sortings and the kind of fillings.",
                  font, Settings::Text_col, Settings::Text_size};

    Text choose_sort = {{20, 65}, "Kinds of sortings:", font, Settings::Text_col, Settings::Text_size};

    Button sort_buttons[5] = {{empty_sort,     "",                      0, false, {30, 100}, 0, font, Settings::Btn_size, Settings::Btn_fill, Settings::Outline_size},
                              {bubble_sort,    Settings::Bubble_str,    1},
                              {selection_sort, Settings::Selection_str, 2},
                              {heap_sort,      Settings::Heap_str,      3},
                              {insertion_sort, Settings::Insertion_str, 4}};

    set_buttons(sort_buttons, Num_types_sortings, 200, 0);

    Text choose_fill = {{20, 150}, "Choose the filling of array for sorting:", font, Settings::Text_col,
                        Settings::Text_size};


    Button fill_buttons[2] = {{empty_sort,  "",                      5, false, {30, 200}, 0, font, Settings::Btn_size, Settings::Btn_fill, Settings::Outline_size},
                              {bubble_sort, Settings::Rand_fill_str, 6}};

    set_buttons(fill_buttons, Num_types_fillings, 200, 0);

    Button next_btn = {bubble_sort, "Start", 7, false, {Settings::Width - 200, Settings::Height - 120}, 1, font};

    Text warning = {{50, 40}, "", font, Settings::Warning_text_color, Settings::Warning_text_size};

    int ready[2] = {};
    bool choose[2][Num_types_sortings] = {};
    bool is_warning = false;
    sf::CircleShape circleShapes[Num_types_sortings][Settings::NMax_for_sort];

    for (auto &circleShape : circleShapes) {
        for (auto &i : circleShape) {
            i = sf::CircleShape(10);
        }
    }

    Graph graph1 = {sf::Vector2f(300, 250),
                    "Dependence of the number of permutations from N", "Num of array's elements", "Num of permutations",
                    font, 1};

    Graph graph2 = {sf::Vector2f(820, 250),
                    "Dependence of the number of comparisons from N", "Num of array's elements", "Num of comparisons",
                    font, 2};


    build_app(&window, &warning_window, sort_buttons, fill_buttons, &next_btn, ready, choose, &brief, &choose_sort,
              &choose_fill, &warning, &is_warning, fixed, &graph1, &graph2);

    return 0;
}
