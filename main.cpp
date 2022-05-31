#include "mbed.h"
#include "ESPAT.h"

Serial pc(USBTX, USBRX);
InterruptIn PIR1(D3); // sensore salotto
InterruptIn PIR2(D6); // sensore bagno
InterruptIn PIR3(D9); // sensore camera da letto

DigitalOut myled1(D7); //led salotto
DigitalOut myled2(D4); //led bagno
DigitalOut myled3(D5); //led camera da letto

int PIR_Detected = 0;
int PIR2_Detected = 0;
int PIR3_Detected = 0;

ESPAT esp(D8, D2, 115200);

void irq_handler(void)
{
    PIR_Detected = 1;
}
void irq_handler2(void)
{
    PIR2_Detected = 1;
}
void irq_handler3(void)
{
    PIR3_Detected = 1;
}



string htmlPage = "<html>" //codice html per accensione da esp
"<head><title>Controllo Led</title></head>"
"<body style=\"background-color:rgb(74, 77, 77);\">"
"<header></header>"
"<h1 style=\"background-color:DodgerBlue;\"><center>Controllo Led</center></h1>"
"<br>"
"<p style=\"color:white; text-align:center; font-size:130%\">Questo sito permette di accendere e spegnere i led collegati alla scherda Nucleo-F411RE. Cliccare su un pulsante per accendere o spegnere i led</p>"
"<br>"
"<center><button onclick=\"fetch('./ledsalaon')\" style=\"background-color:#20B5DF; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Blu ON</b></font></button>"
"<button onclick=\"fetch('./ledsalaoff')\" style=\"background-color:#20B5DF; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Blu OFF</b></font></button></center>"
"<br>"
"<br>"
"<center><button onclick=\"fetch('./ledbagnoon')\" style=\"background-color:#D5E718; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Giallo ON</b></font></button>"
"<button onclick=\"fetch('./ledbagnooff')\" style=\"background-color:#D5E718; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Giallo OFF</b></font></button></center>"
"<br>"
"<br>"
"<center><button onclick=\"fetch('./ledlettoon')\" style=\"background-color:#C43C3C; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Rosso ON</b></font></button>"
"<button onclick=\"fetch('./ledlettooff')\" style=\"background-color:#C43C3C; font-family:courier; width:400px; height:150px;\"><font size=\"6\"><b>Led Rosso OFF</b></font></button></center>"
"<br>"
"<br>"
"<footer><center>Andrea Trapani</center></footer>"
"</body></html>"; // Website code

void handleRequest(int linkId, string path) { // Bei HTTP Request
    if (path == "/") { // Bei direkter IP
        esp.httpReply(linkId, "200 OK", htmlPage); // Website senden
    }else if (path == "/ledsalaon") { // Bei Pfad ledOn/ledOff, Led ein bzw ausschalten
        myled1 = 1;
        esp.httpReply(linkId, "200 OK", "success");
    } else if (path == "/ledsalaoff") {
        myled1 = 0;
        esp.httpReply(linkId, "200 OK", "success");
    }else if (path == "/ledbagnoon") { // Bei Pfad ledOn/ledOff, Led ein bzw ausschalten
        myled2 = 1;
        esp.httpReply(linkId, "200 OK", "success");
    } else if (path == "/ledbagnooff") {
        myled2 = 0;
        esp.httpReply(linkId, "200 OK", "success");
    }else if (path == "/ledlettoon") { // Bei Pfad ledOn/ledOff, Led ein bzw ausschalten
        myled3 = 1;
        esp.httpReply(linkId, "200 OK", "success");
    } else if (path == "/ledlettooff") {
        myled3 = 0;
        esp.httpReply(linkId, "200 OK", "success");
    } else { // Bei unbekannter Seite 404 antworten
        esp.httpReply(linkId, "404 Not Found", "404 Not found!");
    }
}

int main()
{
    
    
    while(1){
        
    //Led0 = Led1 = 1; // Leds aus
    esp.resetEsp(); // ESP Reset
    esp.initWifiStation("casa", "12345678"); // ESP Access Point Initialiseren     
    //Led0 = 0; // LED ein um zu zeigen, dass Bertl aktiv ist
    esp.initServer(handleRequest); // Request Handler initialisieren
        
    PIR1.rise(&irq_handler);
    PIR2.rise(&irq_handler2);
    PIR3.rise(&irq_handler3);

    
        if (PIR_Detected) {
            myled1 = 1;
            PIR_Detected = 0;
            wait(5);
            pc.printf("Movimento rilevato\n\r");
        }else{
        myled1 = 0;
        wait(0.2);
        pc.printf("Movimento non rilevato\n\r");
        }
        
        
        /*if (PIR2_Detected) {
            myled2 = 1;
            PIR2_Detected = 0;
            wait(5);
            pc.printf("Movimento 2 rilevato\n\r");
        }else{
        myled2 = 0;
        wait(0.2);
        pc.printf("Movimento 2 non rilevato\n\r");
        }
        
        
        if (PIR3_Detected) {
            myled3 = 1;
            PIR3_Detected = 0;
            wait(5);
            pc.printf("Movimento 3 rilevato\n\r");
        }else{
        myled3 = 0;
        wait(0.2);
        pc.printf("Movimento 3 non rilevato\n\r");
        }*/
        
    }
}
