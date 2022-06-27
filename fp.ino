//BAHAYA BAWAH
double probBB, meanBB = 10.87, stdevBB = 3.149439842;

//PERINGATAN BAWAH
double probPB, meanPB = 18.97, stdevPB = 1.428389229;

//NORMAL
double probNO, meanNO = 26.85, stdevNO = 3.223024688;

//PERINGATAN ATAS
double probPA, meanPA = 33.69, stdevPA = 0.96095498;

//BAHAYA ATAS
double probBA, meanBA = 38.09, stdevBA = 2.501687588;

float temp, pi = 3.14;

void setup() {
    Serial.begin(115200);
}

void loop() {
    temp = //READ TEMEPERATURE
    
    probBB = (1 / (sqrt(2 * pi * stdevBB))) * exp(-(pow(temp - meanBB, 2) / (pow(2 * stdevBB, 2))));

    probPB = (1 / (sqrt(2 * pi * stdevPB))) * exp(-(pow(temp - meanPB, 2) / (pow(2 * stdevPB, 2))));

    probNO = (1 / (sqrt(2 * pi * stdevNO))) * exp(-(pow(temp - meanNO, 2) / (pow(2 * stdevNO, 2))));

    probPA = (1 / (sqrt(2 * pi * stdevPA))) * exp(-(pow(temp - meanPA, 2) / (pow(2 * stdevPA, 2))));

    probBA = (1 / (sqrt(2 * pi * stdevBA))) * exp(-(pow(temp - meanBA, 2) / (pow(2 * stdevBA, 2))));

    if (probBB > probPB && probBB > probNO && probBB > probPA && probBB > probBA){
        //BAHAYA BAWAH
    }
    if (probPB > probBB && probPB > probNO && probPB > probPA && probPB > probBA){
        //PERINGATAN BAWAH
    }
    if (probNO > probBB && probNO > probPB && probNO > probPA && probNO > probBA){
        //NORMAL
    }
    if (probPA > probBB && probPA > probPB && probPA > probNO && probPA > probBA){
        //PERINGATAN ATAS
    }
    if (probBA > probBB && probBA > probPB && probBA > probNO && probBA > probBA){
        //BAHAYA ATAS
    }


}