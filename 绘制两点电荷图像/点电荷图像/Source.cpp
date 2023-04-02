#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// ��������
const double COULOMB_CONSTANT = 8987551787.3681764; // ���س���
const double POINT_CHARGE_RADIUS = 10.0; // ��ɰ뾶
const double LINE_LENGTH = 100.0; // ���߳���
const double M_PI = 3.1415926;  // ������

// ��������
double distance(double x1, double y1, double x2, double y2);
double coulomb_force(double q1, double q2, double r);

// �����
class Charge {
public:
    Charge(double x, double y, double q) : x(x), y(y), q(q) {}
    double getX() const { return x; }
    double getY() const { return y; }
    double getQ() const { return q; }
private:
    double x, y, q;
};

// ����
class Point {
public:
    Point(double x, double y) : x(x), y(y) {}
    double getX() const { return x; }
    double getY() const { return y; }
private:
    double x, y;
};


int main() {
    // ��������
    sf::RenderWindow window(sf::VideoMode(800, 600), "Electric Field Simulation");

    // ������ɺ͵������
    std::vector<Charge> charges;
    std::vector<Point> points;

    // �����������
    charges.emplace_back(400, 300 - 100, 1.0);
    charges.emplace_back(400, 300 + 100, -1.0);

    while (window.isOpen()) {
        // �����¼�
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // ��մ���
        window.clear();

        // ���Ƶ��
        for (const auto& charge : charges) {
            double x = charge.getX();
            double y = charge.getY();
            double q = charge.getQ();

            sf::CircleShape circle(POINT_CHARGE_RADIUS);
            circle.setFillColor(q > 0 ? sf::Color::Red : sf::Color::Blue);
            circle.setPosition(x - POINT_CHARGE_RADIUS, y - POINT_CHARGE_RADIUS);
            window.draw(circle);
        }

        // ���Ƴ��ߺͳ�����
        for (int i = 0; i < window.getSize().x; i += 10) {
            for (int j = 0; j < window.getSize().y; j += 10) {
                double x = i;
                double y = j;

                double fx = 0.0;
                double fy = 0.0;
                for (const auto& charge : charges) {
                    double x2 = charge.getX();
                    double y2 = charge.getY();
                    double q2 = charge.getQ();

                    double d = distance(x, y, x2, y2);
                    double f = coulomb_force(q2, 1.0, d);

                    fx += f * (x2 - x) / d;
                    fy += f * (y2 - y) / d;
                }

                double length = std::sqrt(fx * fx + fy * fy);
                if (length > 0.0) {
                    // ��������
                    sf::VertexArray line(sf::Lines, 2);
                    line[0].position = sf::Vector2f(x, y);
                    line[1].position = sf::Vector2f(x + LINE_LENGTH * fx / length, y + LINE_LENGTH * fy / length);
                    line[0].color = sf::Color::Green;
                    line[1].color = sf::Color::Green;
                    window.draw(line);

                    // ������
                    sf::VertexArray arc(sf::LinesStrip, 51);
                    double angle = std::atan2(fy, fx);
                    for (int k = 0; k <= 50; ++k) {
                        double r = POINT_CHARGE_RADIUS + LINE_LENGTH * k / 50.0;
                        arc[k].position = sf::Vector2f(x + r * std::cos(angle), y + r * std::sin(angle));
                        arc[k].color = sf::Color::Green;
                        angle += 2.0 * M_PI / 50.0;
                    }
                    window.draw(arc);
                }
            }
        }

        // ��ʾ����
        window.display();
    }


    return 0;
}
// ��������֮��ľ���
double distance(double x1, double y1, double x2, double y2) {
    double dx = x1 - x2;
    double dy = y1 - y2;
    return std::sqrt(dx * dx + dy * dy);
}
// ���������
double coulomb_force(double q1, double q2, double r) {
    return COULOMB_CONSTANT * q1 * q2 / (r * r);
}
