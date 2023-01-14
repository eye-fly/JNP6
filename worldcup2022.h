#ifndef WORLDCUP2022_H
#define WORLDCUP2022_H

#include <vector>
#include <cassert>
#include "worldcup.h"

class Player;
class Field;
class Board;
struct Game;

class WorldCup2022 : public WorldCup {
    public:
    // ~WorldCup2022() override{

    // }

    // Jeżeli argumentem jest pusty wskaźnik, to nie wykonuje żadnej operacji
    // (ale nie ma błędu).
    void addDie(std::shared_ptr<Die> die) override{
        game.dices.push_back(die);
    }

    // Dodaje nowego gracza o podanej nazwie.
     void addPlayer(std::string const &name) override{
        players.push_back(Player(name));
    }

    // Konfiguruje tablicę wyników. Domyślnie jest skonfigurowana tablica
    // wyników, która nic nie robi.
     void setScoreBoard(std::shared_ptr<ScoreBoard> new_scoreboard) override{
        game.scoreboard = new_scoreboard;
    }

    // Przeprowadza rozgrywkę co najwyżej podanej liczby rund (rozgrywka może
    // skończyć się wcześniej).
    // Jedna runda obejmuje po jednym ruchu każdego gracza.
    // Gracze ruszają się w kolejności, w której zostali dodani.
    // Na początku każdej rundy przekazywana jest informacja do tablicy wyników
    // o początku rundy (onRound), a na zakończenie tury gracza informacje
    // podsumowujące dla każdego gracza (onTurn).
    // Rzuca TooManyDiceException, jeśli jest zbyt dużo kostek.
    // Rzuca TooFewDiceException, jeśli nie ma wystarczającej liczby kostek.
    // Rzuca TooManyPlayersException, jeśli jest zbyt dużo graczy.
    // Rzuca TooFewPlayersException, jeśli liczba graczy nie pozwala na
    // rozpoczęcie gry.
    // Wyjątki powinny dziedziczyć po std::exception.
     void play(unsigned int rounds) = 0;

    private:
        std::vector<Player> players;
        Game game;
};

struct Game{
        Board board;
        std::vector<std::shared_ptr<Die>> dices;
        std::shared_ptr<ScoreBoard> scoreboard = NULL;
        uint32_t crr_rounds = 0;
        uint32_t crr_player_index = 0;
};

class Player{
    public:
        Player(std::string const &name): name(name){}

        void play(Game &game){
            if(is_bankrupt) return;
            if(rounds_to_whait == 0){

                assert(game.dices.size() == 2);
                for(int i=0; 2>i; i++) sum_of_moves += game.dices[i].get()->roll();
            
                crr_field = game.board.get_field(sum_of_moves);
                crr_field->stepOn(*this);

            } else rounds_to_whait --;

            game.scoreboard.get()->onTurn(name, getStatus(), crr_field->getName(),money);

        }

        uint32_t money = 1000;
        uint32_t rounds_to_whait = 0;
    private:

        std::string getStatus(){

        }

        std::string name;
        uint32_t sum_of_moves =0;
        bool is_bankrupt = false; 
        Field* crr_field;    
};

class Field{
    public: 
        virtual void stepOn(Player &player) = 0;

        virtual const std::string getName() = 0;
};

class SeazonStart : public Field{
    public:
        void stepOn(Player &player){
            player.money += 50;
        }

        const std::string getName() { return name; }
    private:
         std::string name = "Początek sezonu";
};

class Board{
    public:
        Field* get_field(uint32_t sum_of_moves){
            assert(fields.size() != 0);
            return &fields[sum_of_moves % fields.size()];
        }

    private:
        std::vector<Field> fields = {};
};
#endif