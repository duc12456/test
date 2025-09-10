#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <memory>

class Trail {
private:
    std::vector<sf::Vector2f> points;
    std::vector<sf::CircleShape> trailShapes;
    size_t maxPoints;
    sf::Color baseColor;

public:
    Trail(sf::Color color, size_t maxTrailPoints = 15) 
        : maxPoints(maxTrailPoints), baseColor(color) {}

    void addPoint(sf::Vector2f position) {
        points.push_back(position);
        if (points.size() > maxPoints) {
            points.erase(points.begin());
        }
    }

    void draw(sf::RenderWindow& window) {
        trailShapes.clear();
        for (size_t i = 0; i < points.size(); ++i) {
            float alpha = static_cast<float>(i) / static_cast<float>(points.size());
            float radius = alpha * 8.0f + 2.0f;
            
            sf::CircleShape trailPoint(radius);
            trailPoint.setPosition(points[i].x - radius, points[i].y - radius);
            
            sf::Color trailColor = baseColor;
            trailColor.a = static_cast<sf::Uint8>(alpha * 120);
            trailPoint.setFillColor(trailColor);
            
            window.draw(trailPoint);
        }
    }

    void clear() {
        points.clear();
    }
};

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Color originalColor;
    float pulseTimer;
    float rotationSpeed;
    float currentRotation;
    std::unique_ptr<Trail> trail;
    
    // Glow effect properties
    sf::CircleShape glowShape;
    sf::CircleShape innerGlow;
    
    Ball(float x, float y, float radius, sf::Color color) {
        shape.setRadius(radius);
        shape.setPosition(x, y);
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
        originalColor = color;

        // Create glow effects
        glowShape.setRadius(radius * 2.0f);
        glowShape.setOrigin(radius * 2.0f, radius * 2.0f);
        glowShape.setPosition(x, y);
        
        innerGlow.setRadius(radius * 1.3f);
        innerGlow.setOrigin(radius * 1.3f, radius * 1.3f);
        innerGlow.setPosition(x, y);

        // Random velocity
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-400.0f, 400.0f);
        velocity.x = dis(gen);
        velocity.y = dis(gen);

        // Random rotation speed
        std::uniform_real_distribution<float> rotDis(-180.0f, 180.0f);
        rotationSpeed = rotDis(gen);

        pulseTimer = 0.0f;
        currentRotation = 0.0f;

        // Initialize trail
        trail = std::make_unique<Trail>(color);
    }

    void update(float deltaTime, sf::Vector2u windowSize) {
        // Update position
        sf::Vector2f oldPos = shape.getPosition();
        shape.move(velocity * deltaTime);
        
        // Add position to trail
        trail->addPoint(shape.getPosition());

        // Bounce off walls
        sf::Vector2f pos = shape.getPosition();
        float radius = shape.getRadius();
        
        bool bounced = false;
        if (pos.x - radius <= 0 || pos.x + radius >= windowSize.x) {
            velocity.x = -velocity.x * 0.98f; // Energy loss
            bounced = true;
        }
        if (pos.y - radius <= 0 || pos.y + radius >= windowSize.y) {
            velocity.y = -velocity.y * 0.98f;
            bounced = true;
        }

        if (bounced) {
            createSparkEffect();
            // Add screen shake effect (you can implement this in Game class)
        }

        // Keep ball in bounds
        if (pos.x - radius < 0) shape.setPosition(radius, pos.y);
        if (pos.x + radius > windowSize.x) shape.setPosition(windowSize.x - radius, pos.y);
        if (pos.y - radius < 0) shape.setPosition(pos.x, radius);
        if (pos.y + radius > windowSize.y) shape.setPosition(pos.x, windowSize.y - radius);

        // Update glow positions
        glowShape.setPosition(shape.getPosition());
        innerGlow.setPosition(shape.getPosition());

        // Pulsing animation
        pulseTimer += deltaTime * 4.0f;
        float pulse = (std::sin(pulseTimer) + 1.0f) * 0.5f;
        
        // Color animation
        sf::Uint8 alpha = static_cast<sf::Uint8>(180 + pulse * 75);
        sf::Color currentColor = originalColor;
        currentColor.a = alpha;
        shape.setFillColor(currentColor);

        // Rotation
        currentRotation += rotationSpeed * deltaTime;
        shape.setRotation(currentRotation);

        // Glow effects
        sf::Color glowColor = originalColor;
        glowColor.a = static_cast<sf::Uint8>(30 + pulse * 50);
        glowShape.setFillColor(glowColor);
        
        sf::Color innerGlowColor = originalColor;
        innerGlowColor.a = static_cast<sf::Uint8>(60 + pulse * 90);
        innerGlow.setFillColor(innerGlowColor);

        // Dynamic size pulsing
        float sizeMultiplier = 1.0f + pulse * 0.2f;
        glowShape.setScale(sizeMultiplier, sizeMultiplier);
        innerGlow.setScale(sizeMultiplier * 0.8f, sizeMultiplier * 0.8f);
    }

    void draw(sf::RenderWindow& window) {
        // Draw trail first
        trail->draw(window);
        
        // Draw glow effects
        window.draw(glowShape);
        window.draw(innerGlow);
        
        // Draw main ball
        window.draw(shape);
    }

private:
    void createSparkEffect() {
        // Enhanced color change on bounce with rainbow effect
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> hue(0.0f, 360.0f);
        
        float h = hue(gen);
        float s = 0.8f;
        float v = 1.0f;
        
        // HSV to RGB conversion
        sf::Color newColor = hsvToRgb(h, s, v);
        originalColor = newColor;
        
        // Clear trail for visual effect
        trail->clear();
    }

    sf::Color hsvToRgb(float h, float s, float v) {
        float c = v * s;
        float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        
        float r, g, b;
        if (h >= 0 && h < 60) {
            r = c; g = x; b = 0;
        } else if (h >= 60 && h < 120) {
            r = x; g = c; b = 0;
        } else if (h >= 120 && h < 180) {
            r = 0; g = c; b = x;
        } else if (h >= 180 && h < 240) {
            r = 0; g = x; b = c;
        } else if (h >= 240 && h < 300) {
            r = x; g = 0; b = c;
        } else {
            r = c; g = 0; b = x;
        }
        
        return sf::Color(
            static_cast<sf::Uint8>((r + m) * 255),
            static_cast<sf::Uint8>((g + m) * 255),
            static_cast<sf::Uint8>((b + m) * 255)
        );
    }
};

class EnhancedParticle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    float lifetime;
    float maxLifetime;
    sf::Color startColor;
    float startSize;
    float gravity;

    EnhancedParticle(sf::Vector2f position, sf::Color color) {
        startSize = 1.0f + static_cast<float>(rand()) / RAND_MAX * 4.0f;
        shape.setRadius(startSize);
        shape.setPosition(position);
        shape.setOrigin(startSize, startSize);
        startColor = color;
        shape.setFillColor(color);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> velDis(-200.0f, 200.0f);
        std::uniform_real_distribution<float> gravityDis(50.0f, 150.0f);
        
        velocity.x = velDis(gen);
        velocity.y = velDis(gen);
        acceleration = sf::Vector2f(0, 0);
        gravity = gravityDis(gen);
        
        lifetime = 0.0f;
        maxLifetime = 1.5f + static_cast<float>(rand()) / RAND_MAX * 1.5f;
    }

    bool update(float deltaTime) {
        lifetime += deltaTime;
        
        // Apply gravity
        acceleration.y = gravity;
        velocity += acceleration * deltaTime;
        shape.move(velocity * deltaTime);

        // Fade out and shrink
        float lifeRatio = lifetime / maxLifetime;
        float alpha = (1.0f - lifeRatio) * 255.0f;
        float size = startSize * (1.0f - lifeRatio * 0.5f);
        
        sf::Color currentColor = startColor;
        currentColor.a = static_cast<sf::Uint8>(std::max(0.0f, alpha));
        shape.setFillColor(currentColor);
        
        shape.setRadius(std::max(0.1f, size));

        return lifetime < maxLifetime;
    }
};

class EnhancedParticleSystem {
public:
    std::vector<EnhancedParticle> particles;

    void addParticle(sf::Vector2f position, sf::Color color) {
        particles.emplace_back(position, color);
    }

    void addExplosion(sf::Vector2f position, sf::Color color, int count = 15) {
        for (int i = 0; i < count; ++i) {
            addParticle(position, color);
        }
    }

    void update(float deltaTime) {
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                [deltaTime](EnhancedParticle& p) {
                    return !p.update(deltaTime);
                }),
            particles.end()
        );
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& particle : particles) {
            window.draw(particle.shape);
        }
    }

    size_t getCount() const {
        return particles.size();
    }
};

class Game {
private:
    sf::RenderWindow window;
    std::vector<std::unique_ptr<Ball>> balls;
    EnhancedParticleSystem particleSystem;
    sf::Clock clock;
    sf::Font font;
    sf::Text titleText;
    sf::Text statsText;
    
    // Enhanced background
    sf::RectangleShape background;
    std::vector<sf::CircleShape> backgroundStars;
    float backgroundTimer;
    
    // Screen shake
    sf::Vector2f shakeOffset;
    float shakeIntensity;
    float shakeTimer;

public:
    Game() : window(sf::VideoMode(1400, 900), "Enhanced Bouncing Balls - Beautiful C++ Animation", 
                    sf::Style::Close | sf::Style::Titlebar) {
        
        window.setFramerateLimit(60);
        backgroundTimer = 0.0f;
        shakeIntensity = 0.0f;
        shakeTimer = 0.0f;

        // Create animated background
        background.setSize(sf::Vector2f(1400, 900));
        createBackgroundStars();

        // Create initial balls with premium colors
        std::vector<sf::Color> premiumColors = {
            sf::Color(255, 20, 147),   // Deep Pink
            sf::Color(0, 191, 255),    // Deep Sky Blue
            sf::Color(50, 205, 50),    // Lime Green
            sf::Color(255, 165, 0),    // Orange
            sf::Color(138, 43, 226),   // Blue Violet
            sf::Color(255, 69, 0),     // Red Orange
            sf::Color(0, 255, 127),    // Spring Green
            sf::Color(255, 20, 147),   // Hot Pink
            sf::Color(64, 224, 208),   // Turquoise
            sf::Color(220, 20, 60)     // Crimson
        };

        for (int i = 0; i < 6; ++i) {
            float x = 150 + (i % 3) * 400;
            float y = 150 + (i / 3) * 300;
            float radius = 25 + (i % 3) * 15;
            balls.push_back(std::make_unique<Ball>(x, y, radius, 
                           premiumColors[i % premiumColors.size()]));
        }

        // Setup fonts and UI
        setupUI();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void createBackgroundStars() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> posDis(0.0f, 1400.0f);
        std::uniform_real_distribution<float> sizeDis(0.5f, 2.5f);

        for (int i = 0; i < 100; ++i) {
            sf::CircleShape star(sizeDis(gen));
            star.setPosition(posDis(gen), posDis(gen));
            star.setFillColor(sf::Color(255, 255, 255, 30 + rand() % 50));
            backgroundStars.push_back(star);
        }
    }

    void setupUI() {
        // Try to load font
        if (!font.loadFromFile("arial.ttf")) {
            // Use default font if loading fails
        }

        titleText.setFont(font);
        titleText.setString("Enhanced Bouncing Balls");
        titleText.setCharacterSize(48);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(20, 20);
        titleText.setStyle(sf::Text::Bold);

        statsText.setFont(font);
        statsText.setCharacterSize(20);
        statsText.setFillColor(sf::Color(200, 200, 200));
        statsText.setPosition(20, 80);
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    
                    // Create new ball with random premium color
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<float> hue(0.0f, 360.0f);
                    
                    float h = hue(gen);
                    sf::Color randomColor = hsvToRgb(h, 0.8f, 1.0f);
                    
                    balls.push_back(std::make_unique<Ball>(
                        static_cast<float>(mousePos.x), 
                        static_cast<float>(mousePos.y), 
                        20 + rand() % 20, 
                        randomColor
                    ));

                    // Add explosion effect
                    particleSystem.addExplosion(
                        sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)), 
                        randomColor, 20
                    );

                    // Add screen shake
                    shakeIntensity = 10.0f;
                    shakeTimer = 0.3f;
                }
            }

            // Clear all balls on right click
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Right) {
                    balls.clear();
                    particleSystem = EnhancedParticleSystem(); // Reset particles
                }
            }
        }
    }

    void update() {
        float deltaTime = clock.restart().asSeconds();
        backgroundTimer += deltaTime;

        // Update screen shake
        if (shakeTimer > 0) {
            shakeTimer -= deltaTime;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> shakeDis(-1.0f, 1.0f);
            
            shakeOffset.x = shakeDis(gen) * shakeIntensity;
            shakeOffset.y = shakeDis(gen) * shakeIntensity;
            shakeIntensity *= 0.95f; // Decay
        } else {
            shakeOffset = sf::Vector2f(0, 0);
        }

        // Animate background
        updateBackground();

        // Update all balls
        for (auto& ball : balls) {
            ball->update(deltaTime, window.getSize());
        }

        // Update particle system
        particleSystem.update(deltaTime);

        // Update UI
        updateUI();
    }

    void updateBackground() {
        // Dynamic background color
        float r = (std::sin(backgroundTimer * 0.3f) + 1.0f) * 0.5f * 30.0f + 5.0f;
        float g = (std::sin(backgroundTimer * 0.2f + 2.0f) + 1.0f) * 0.5f * 30.0f + 10.0f;
        float b = (std::sin(backgroundTimer * 0.4f + 4.0f) + 1.0f) * 0.5f * 40.0f + 15.0f;
        background.setFillColor(sf::Color(static_cast<sf::Uint8>(r), 
                                         static_cast<sf::Uint8>(g), 
                                         static_cast<sf::Uint8>(b)));

        // Animate background stars
        for (auto& star : backgroundStars) {
            float twinkle = (std::sin(backgroundTimer * 2.0f + star.getPosition().x * 0.01f) + 1.0f) * 0.5f;
            sf::Uint8 alpha = static_cast<sf::Uint8>(20 + twinkle * 60);
            sf::Color starColor = star.getFillColor();
            starColor.a = alpha;
            star.setFillColor(starColor);
        }
    }

    void updateUI() {
        // Animated title
        float titlePulse = (std::sin(backgroundTimer * 1.5f) + 1.0f) * 0.5f;
        sf::Uint8 titleAlpha = static_cast<sf::Uint8>(180 + titlePulse * 75);
        titleText.setFillColor(sf::Color(255, 255, 255, titleAlpha));

        // Stats
        std::string stats = "Balls: " + std::to_string(balls.size()) + 
                           " | Particles: " + std::to_string(particleSystem.getCount()) +
                           "\nLeft Click: Add Ball | Right Click: Clear All";
        statsText.setString(stats);
    }

    void render() {
        window.clear();

        // Apply screen shake
        sf::View shakeView = window.getDefaultView();
        shakeView.move(shakeOffset);
        window.setView(shakeView);

        // Draw background
        window.draw(background);
        for (const auto& star : backgroundStars) {
            window.draw(star);
        }

        // Draw all balls
        for (const auto& ball : balls) {
            ball->draw(window);
        }

        // Draw particles
        particleSystem.draw(window);

        // Reset view for UI
        window.setView(window.getDefaultView());

        // Draw UI
        window.draw(titleText);
        window.draw(statsText);

        window.display();
    }

    sf::Color hsvToRgb(float h, float s, float v) {
        float c = v * s;
        float x = c * (1 - std::abs(std::fmod(h / 60.0f, 2) - 1));
        float m = v - c;
        
        float r, g, b;
        if (h >= 0 && h < 60) {
            r = c; g = x; b = 0;
        } else if (h >= 60 && h < 120) {
            r = x; g = c; b = 0;
        } else if (h >= 120 && h < 180) {
            r = 0; g = c; b = x;
        } else if (h >= 180 && h < 240) {
            r = 0; g = x; b = c;
        } else if (h >= 240 && h < 300) {
            r = x; g = 0; b = c;
        } else {
            r = c; g = 0; b = x;
        }
        
        return sf::Color(
            static_cast<sf::Uint8>((r + m) * 255),
            static_cast<sf::Uint8>((g + m) * 255),
            static_cast<sf::Uint8>((b + m) * 255)
        );
    }
};

int main() {
    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
