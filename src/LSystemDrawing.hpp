#pragma once

#include <queue>
#include <algorithm>
#include <cmath>
#include <stack>

#include "raylib.h"

template <typename SymbolType>
struct DrawRuleStruct {
    const SymbolType symbolType{};

    float draw_line_size{0};
    bool end_this_branch{false};
    float turn_angle{0}; // Positive or negative angle, in radians

    bool push_fifo{};
    bool pop_fifo{};
};

template <typename SymbolType>
class LSystemDrawing {
public:
    LSystemDrawing() = default;
    LSystemDrawing(const std::vector<DrawRuleStruct<SymbolType>>& draw_rules, float screen_width, float screen_height);

    DrawRuleStruct<SymbolType> DrawruleFromSymbol(const SymbolType& symbol) const;
    void Draw(const std::vector<SymbolType>& input, float size_multiplier = 1.0);
private:
    const float line_thickness{5.0f};
    const float screen_width{};
    const float screen_height{};

    std::vector<DrawRuleStruct<SymbolType>> draw_rules;
};

template<typename SymbolType>
LSystemDrawing<SymbolType>::LSystemDrawing(const std::vector<DrawRuleStruct<SymbolType>>& draw_rules, float screen_width, float screen_height): draw_rules(draw_rules), screen_height(screen_height), screen_width(screen_width) {

}

template<typename SymbolType>
DrawRuleStruct<SymbolType> LSystemDrawing<SymbolType>::DrawruleFromSymbol(const SymbolType &symbol) const {
    const auto draw_rule_iter = std::find_if(this->draw_rules.begin(), this->draw_rules.end(),
                                          [symbol](const DrawRuleStruct<SymbolType>& rule_struct) { return symbol == rule_struct.symbolType;
        }
    );
    if (draw_rule_iter == this->draw_rules.end()) {
        return {};
    }
    return *draw_rule_iter;
}

template<typename SymbolType>
void LSystemDrawing<SymbolType>::Draw(const std::vector<SymbolType>& input, const float size_multiplier) {
    // Working variables
    std::stack<std::pair<Vector2, float>> lifo{};
    Vector2 current_pos = {screen_width / 2, screen_height - 20.0f};
    float current_angle = 0;

    // Drawing
    for (const auto &symbol: input) {
        auto draw_rule = this->DrawruleFromSymbol(symbol);
        auto line_size = draw_rule.draw_line_size * size_multiplier;

        // Fifo
        if (draw_rule.push_fifo) {
            lifo.emplace(current_pos, current_angle);
        }
        if (draw_rule.pop_fifo) {
            auto[saved_pos, saved_angle] = lifo.top();
            current_pos = saved_pos;
            current_angle = saved_angle;
            lifo.pop();
        }

        // Constructing next position
        const float angle = current_angle + draw_rule.turn_angle;
        const float x_diff = line_size * sinf(angle);
        const float y_diff = line_size * cosf(angle);
        const Vector2 next_pos = {current_pos.x + x_diff, current_pos.y - y_diff};

        // Drawing line
        DrawLineEx(
                next_pos, current_pos, this->line_thickness, DARKGRAY
                );

        // Saving state
        if (not draw_rule.end_this_branch) {
            current_angle = angle;
            current_pos = next_pos;
        }
    }
}
