#include <curl/curl.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

struct Player {
    string standard, rapid, blitz;
    string name;
};

int maxName = 0;

ostream& operator<<(ostream& os, const Player& player){
    int extraSpace = 3;
    extraSpace += (maxName - player.name.length());
    os << player.name;
    while(extraSpace > 0){
        os << " ";
        extraSpace--;
    }
    return os << player.standard << "   " << player.rapid << "   " << player.blitz << '\n';
}

int main(void) {

    CURL* easyhandle = curl_easy_init();

    string readBuffer;

    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(easyhandle, CURLOPT_URL, "https://2700chess.com/");

    CURLcode result = curl_easy_perform(easyhandle);

    curl_easy_cleanup(easyhandle);


    vector<Player> players;

    while(readBuffer.length() > 0) {

        Player player = {};

        int here = readBuffer.find("<td class=\"name\"");

        if(here < 0)
            break;

        readBuffer = readBuffer.substr(here);
        here = readBuffer.find("\"hidden searched\">");
        readBuffer = readBuffer.substr(here);
        auto start = readBuffer.find('>') + 1;
        auto end = readBuffer.find('<');
        player.name = readBuffer.substr(start, end - start);
        if(player.name.length() > maxName){
            maxName = player.name.length();
        }

        here = readBuffer.find("live_standard_rating");
        readBuffer = readBuffer.substr(here + 40);
        start = readBuffer.find('>') + 1;
        end = readBuffer.find('<');
        player.standard = readBuffer.substr(start, end - start);
        readBuffer = readBuffer.substr(here + 10);

        here = readBuffer.find("live_rapid_rating");
        readBuffer = readBuffer.substr(here + 50);
        start = readBuffer.find('>') + 1;
        end = readBuffer.find('<');
        player.rapid = readBuffer.substr(start, end - start);
        readBuffer = readBuffer.substr(here + 10);

        here = readBuffer.find("live_blitz_rating");
        readBuffer = readBuffer.substr(here + 50);
        start = readBuffer.find('>') + 1;
        end = readBuffer.find('<');
        player.blitz = readBuffer.substr(start, end - start);

        players.push_back(player);
    }

    string n = "Name:";

    cout << n;
    int extraSpace = 3;
    extraSpace += (maxName - n.length());
    while(extraSpace > 0){
        cout << " ";
        extraSpace--;
    }

    cout<< "Std:     Rapid:   Blitz:\n";

    extraSpace = maxName + 3;
    while(extraSpace > 0){
        cout << "-";
        extraSpace--;
    }
    cout<< "------------------------\n";
    for(Player p : players){
        cout << p;
    }


    return 0;
}