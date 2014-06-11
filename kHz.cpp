#include <iostream>
#include <fstream>
#include <cstdint>
#include <math.h>

#define PI 3.14159265

using namespace std;

struct WaveHeader{

    //RIFF Chunk descriptor
    char chunkID[4] = {'R','I','F','F'}; //ChunkID
    uint32_t chunkSize;//ChunkSize
    char WAVE[4] = {'W','A','V','E'}; //Format
    //fmt sub chunk
    char subChunkID[4] = {'f','m','t',' '}; //subChunkID
    uint32_t subChunk1Size = 16; //16-bit audio
    uint16_t audioFormat = 1; //Linear PCM
    uint16_t numChannels = 2; //stereo
    uint32_t sampleRate = 44100; //44.1kHz sample rate
    uint32_t byteRate = (sampleRate * numChannels * 16)/8; //pretty obvious
    uint16_t blockAlign = 4;
    uint16_t bitsPerSample = 16;
    //data subchunk
    char subChunk2ID[4] = {'d','a','t','a'}; //"data" header
    uint32_t subChunk2Size;

};

int main(){

    WaveHeader wave;
    ofstream myWav;
    double x;
    int16_t sampBuff = 0;
    //int sec = 0, freq = 0, sampL = 0, sampR = 0;
    //uint8_t numb = 255;

    myWav.open("test.wav", ios::out | ios::binary);

    if(!myWav){

        cout << "Unable to create file!\n";
        return 0;

    }

    cout << "Creating file...\n";
    cout << "Generating sine...\n";

    myWav.seekp(sizeof(wave), ios::beg);

    for(int i = 0; i < 44100; i++){

        x = i/44100.0;
        sampBuff = ((0.5*(sin(x*2.0*PI*1000))) * 32767);
        myWav.write(reinterpret_cast<char *>(&sampBuff), sizeof(sampBuff)); //Left Channel
        myWav.write(reinterpret_cast<char *>(&sampBuff), sizeof(sampBuff)); //Right Channel
        wave.subChunk2Size = (i + 1) * 4;
        cout << "\rsampBuff: " << sampBuff << std::flush;

    }

        wave.chunkSize = wave.subChunk2Size + 36;

        cout << "\nFinalizing...";

        myWav.seekp(0, ios::beg);
        myWav.write(reinterpret_cast<char *>(&wave), sizeof(wave));

    //myWav.write(reinterpret_cast<char *>(&numb), sizeof(numb));
    //myWav.write(reinterpret_cast<char *>(&wave), sizeof(wave));
    myWav.close();

    cout << "\n\n...File successfully created!\n";

    return 0;


}
