#include <sdk.h>
#include <stdlib.h>
#include <bls/bls.hpp>
#include <string>

#define UNUSED(x) (void)(x)
void* memset(void *ptr, int a, size_t b) {
    char* dest = (char*) ptr;
    for (size_t t = 0; t < b; ++t) {
        *(dest + t) = a;
    }
}

void init() {
    static bool inited = false;

    if (inited)
        return;

    bls::init();
}

extern "C" {
char *invoke(const char *str, int length) {
    init();

//    bls::Signature s;
    bls::PublicKey pk;

    pk.setStr(std::string(str));

    std::string string;
    pk.getStr(string);

    return (char*)string.data();
}

static int game_id_autoinc = 0;


struct pub_key_type {
    char *data;
    int len;
};

struct Game {
    bool new_game(int n) {
        len = 0;
        players = (char**)malloc(sizeof(char*) * n);
        stakes = (int*)malloc(sizeof(int) * n);
        game_id = game_id_autoinc++;
        return true;
    }

    bool add_stake(int stake, pub_key_type pub_key) {
        stakes[len] = stake;
        players[len] = pub_key.data;
        ++len;
    }

    int get_stake(pub_key_type pub_key) {
        for (int i = 0; i < len; i++) {
            if (compare(players[i], pub_key.data, pub_key.len))
                return stakes[i];
        }
        return 0;
    }

    bool compare(char *a, char *b, int len) {
        for (int i = 0; i < len; i++) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }


    int get_game_id() {
        return game_id;
    }

    char** get_participants() {
        return players;
    }
private:
    int *stakes;
    char **players;
    int len;
    int game_id;
};


}
