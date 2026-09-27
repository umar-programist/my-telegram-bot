#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string httpGet(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

std::string urlEncode(CURL* curl, const std::string& value) {
    if (!curl) return value;
    char* output = curl_easy_escape(curl, value.c_str(), value.length());
    if (output) {
        std::string result(output);
        curl_free(output);
        return result;
    }
    return value;
}

int main() {
    std::string token = "8265671487:AAE6drx6DaVahd06tcSfsItQEGGDy1z9-Gc";
    long long last_update_id = 0;
    int total_users = 0;

    CURL* curl = curl_easy_init();

    std::cout << "========================================" << std::endl;
    std::cout << " БОТ УСПЕШНО ЗАПУЩЕН!                   " << std::endl;
    std::cout << "========================================" << std::endl;

    json main_keyboard;
    main_keyboard["resize_keyboard"] = true;
    json kbd_rows = json::array();

    json row1 = json::array();
    row1.push_back({{"text", "🌐 Портфолио ва Сайт"}});
    row1.push_back({{"text", "💼 Фармоиши сайт / бот"}});

    json row2 = json::array();
    row2.push_back({{"text", "👥 Системаи рефералӣ"}});
    row2.push_back({{"text", "💳 Пардохт ва Тарифҳо"}});

    json row3 = json::array();
    row3.push_back({{"text", "📞 Алоқа бо дастурнавис"}});

    kbd_rows.push_back(row1);
    kbd_rows.push_back(row2);
    kbd_rows.push_back(row3);
    main_keyboard["keyboard"] = kbd_rows;

    while (true) {
        std::string url = "https://api.telegram.org/bot" + token + "/getUpdates?offset=" + std::to_string(last_update_id + 1) + "&timeout=5";
        std::string response = httpGet(url);

        if (!response.empty()) {
            try {
                auto j = json::parse(response);
                if (j.is_object() && j.contains("ok") && j["ok"] == true && j.contains("result")) {
                    for (const auto& item : j["result"]) {
                        last_update_id = item["update_id"];

                        if (item.contains("message") && item["message"].contains("text")) {
                            long long chat_id = item["message"]["chat"]["id"];
                            std::string text = item["message"]["text"];
                            std::string first_name = "Дӯст";
                            
                            if (item["message"].contains("from") && item["message"]["from"].contains("first_name")) {
                                first_name = item["message"]["from"]["first_name"].get<std::string>();
                            }

                            std::string reply_text;
                            json reply_markup = main_keyboard;

                            if (text == "/start" || text == "⬅️ Баргаштан ба меню") {
                                total_users++;
                                reply_text = "Салом, " + first_name + "!\n\nХуш омадед ба боти хизматрасониҳои барномасозӣ.\nАз менюи зер бахши дилхоҳро интихоб кунед:";
                            } 
                            else if (text == "🌐 Портфолио ва Сайт") {
                                reply_text = "👨‍💻 Портфолиои расмии UMAR:\n\nДар ин ҷо лоиҳаҳо ва сайтҳои сохташударо дида метавонед.";
                                json inline_kbd;
                                json irows = json::array();
                                json irow = json::array();
                                irow.push_back({{"text", "🔗 Кушодани сайт (Портфолио)"}, {"url", "https://umar-programist.github.io/Developer-Portfoilo/index2.html"}});
                                irows.push_back(irow);
                                inline_kbd["inline_keyboard"] = irows;
                                reply_markup = inline_kbd;
                            }
                            else if (text == "💼 Фармоиши сайт / бот") {
                                reply_text = "🛠 Хидматҳои пулакии мо:\n\n"
                                             "1. Сохтани вебсайтҳои муосир (Landing page, Портфолио)\n"
                                             "2. Сохтани ботҳои Телеграм (C++, Python, JS)\n"
                                             "3. Автоматизатсияи тиҷорат\n\n"
                                             "Барои фармоиш додан ба админ нависед:";
                                json inline_kbd;
                                json irows = json::array();
                                json irow = json::array();
                                irow.push_back({{"text", "📝 Навиштан ба UMAR"}, {"url", "https://t.me/UMARSUFIEV"}});
                                irows.push_back(irow);
                                inline_kbd["inline_keyboard"] = irows;
                                reply_markup = inline_kbd;
                            }
                            else if (text == "👥 Системаи рефералӣ") {
                                std::string ref_link = "https://t.me/WebCoder2008Bot?start=" + std::to_string(chat_id);
                                reply_text = "💰 Барномаи рефералӣ:\n\n"
                                             "Истиноди худро ба дӯстонатон фиристед. Барои ҳар як дӯсти даъваткардаатон бонус ва таъхфиф мегиред!\n\n"
                                             "🔗 Истиноди Шумо:\n" + ref_link;
                                reply_markup = main_keyboard;
                            }
                            else if (text == "💳 Пардохт ва Тарифҳо") {
                                reply_text = "💳 Нархномаи хизматрасониҳо:\n\n"
                                             "• Боти оддии Телеграм: аз 100 сомонӣ\n"
                                             "• Боти мураккаб / Мағоза: аз 300 сомонӣ\n"
                                             "• Вебсайти визитка: аз 250 сомонӣ\n\n"
                                             "Пардохт тавассути Алиф / Душанбе Сити қабул карда мешавад.";
                                json inline_kbd;
                                json irows = json::array();
                                json irow = json::array();
                                irow.push_back({{"text", "💳 Пардохт кардан (Алоқа)"}, {"url", "https://t.me/UMARSUFIEV"}});
                                irows.push_back(irow);
                                inline_kbd["inline_keyboard"] = irows;
                                reply_markup = inline_kbd;
                            }
                            else if (text == "📞 Алоқа бо дастурнавис") {
                                reply_text = "Савол ё пешниҳоде доред? Мустақиман ба эҷодкори бот нависед:";
                                json inline_kbd;
                                json irows = json::array();
                                json irow = json::array();
                                irow.push_back({{"text", "💬 Навиштан ба @UMARSUFIEV"}, {"url", "https://t.me/UMARSUFIEV"}});
                                irows.push_back(irow);
                                inline_kbd["inline_keyboard"] = irows;
                                reply_markup = inline_kbd;
                            }
                            else if (text == "/admin") {
                                reply_text = "📊 Панели админ:\n\nМиқдори корбарони фаъол: " + std::to_string(total_users);
                                reply_markup = main_keyboard;
                            }
                            else {
                                reply_text = "Лутфан яке аз тугмаҳои менюро интихоб кунед.";
                                reply_markup = main_keyboard;
                            }

                            std::string encoded_text = urlEncode(curl, reply_text);
                            std::string send_url = "https://api.telegram.org/bot" + token + "/sendMessage?chat_id=" + std::to_string(chat_id) + "&text=" + encoded_text;

                            if (!reply_markup.empty()) {
                                std::string encoded_markup = urlEncode(curl, reply_markup.dump());
                                send_url += "&reply_markup=" + encoded_markup;
                            }

                            httpGet(send_url);
                        }
                    }
                }
            } catch (...) {}
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    if (curl) curl_easy_cleanup(curl);
    return 0;
}
