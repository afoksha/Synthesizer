#include <iostream>
#include <cmath>
#include <random>
#include <string>
#include <algorithm>
#include <array>
#include <math.h>

static const int one = 1;
static bool isLE = (*(const uint8_t*)(&one));
const int sampleRate = 8192;

float frequency(int note, int o)
{
    return 16.351786f * pow(2.0f, o + note / 12.0f);
}

//=============================================================================================================================================================================
//=============================================================================================================================================================================
//=============================================================================================================================================================================

struct note_t
{
    float duration;
    int octave;
    int note;

    note_t(float duration, int octave, int note) :
        duration(duration), octave(octave), note(note) {}
};

note_t moonlight_sonata[] =
{
    note_t(1.0f / 4.0f, 2, 2),
    note_t(1.0f / 4.0f, 3, 2),
    note_t(1.0f / 4.0f, 3, 5),
    note_t(1.0f / 4.0f, 3, 9),
    note_t(1.0f / 8.0f, 2, 2),
    note_t(1.0f / 8.0f, 2, 4),
    note_t(1.0f / 4.0f, 2, 5),
    note_t(1.0f / 4.0f, 2, 4),
    note_t(1.0f / 4.0f, 2, 0),
    note_t(1.0f / 4.0f, 3, 7),
    note_t(1.0f / 8.0f, 3, 5),
    note_t(1.0f / 8.0f, 3, 7),
    note_t(1.0f / 4.0f, 3, 9),
    note_t(1.0f / 4.0f, 3, 2),
    note_t(1.0f / 4.0f, 3, 5),
    note_t(1.0f / 4.0f, 3, 9)
};

const int sonata_length = sizeof(moonlight_sonata) / sizeof(note_t);

struct WAV_writer
{
    // I don't think it's at all my fault that some players
    // claim to support WAV format, but fail to play a WAV
    // file that has lower sample rate than "standard".

    FILE* wavefile = 0;

    bool create(const std::string& filename)
    {
        wavefile = std::fopen(filename.c_str(), "wb");
        if (wavefile == 0)
            return false;

        if (isLE)
            std::fprintf(wavefile, "RIFF");                         //ASCII for 0x52494646, the magic number that WAV files start with
        else
            std::fprintf(wavefile, "RIFX");                         //Big endian WAV file starts with magic number 0x52494658, or, in ASCII, "RIFX"

        int32_t ChunkSize = 36 + 8 * sampleRate * 2;
        std::fwrite(&ChunkSize, 4, 1, wavefile);
        std::fprintf(wavefile, "WAVEfmt ");                         //The beginning of the header
        int32_t Subchunk1Size = 16;                                 //PCM header is always 16 bytes
        std::fwrite(&Subchunk1Size, 4, 1, wavefile);
        int16_t AudioFormat = 1;                                    //PCM format
        std::fwrite(&AudioFormat, 2, 1, wavefile);
        int16_t NumChannels = 1;                                    //MONO audio
        std::fwrite(&NumChannels, 2, 1, wavefile);
        int32_t SampleRate = sampleRate;
        std::fwrite(&SampleRate, 4, 1, wavefile);
        int32_t ByteRate = 2 * sampleRate;                          //Since we are using 16 bits per sample and "sampleRate" samples per second
        std::fwrite(&ByteRate, 4, 1, wavefile);
        int16_t BlockAlign = 2;                                     //Each block is two bytes
        std::fwrite(&BlockAlign, 2, 1, wavefile);
        int16_t BitsPerSample = 16;
        std::fwrite(&BitsPerSample, 2, 1, wavefile);
        std::fprintf(wavefile, "data");

        return true;
    }

    void fill_note(float duration, float frequency)
    {
        int noteDuration = sampleRate * duration * 2.0f;

        for (int i = 0; i < noteDuration; i++)
        {
            float baseFrequency = std::sin(2 * M_PI * 1 * frequency * i / sampleRate) * std::pow(2, 14);
            float secondHarmony = std::sin(2 * M_PI * 2 * frequency * i / sampleRate + M_PI / 4) * std::pow(2, 12);
            float thirdHarmony  = std::sin(2 * M_PI * 3 * frequency * i / sampleRate + M_PI / 2) * std::pow(2, 10);
            float fourthHarmony = std::sin(2 * M_PI * 4 * frequency * i / sampleRate - M_PI / 4) * std::pow(2, 9);
            float currentAmplitude = (baseFrequency + secondHarmony + thirdHarmony + fourthHarmony) * std::exp(-(float)(1.25f * i) / (sampleRate)); // Attenuation.
            int16_t numberToBeWritten = currentAmplitude + std::rand() % (1 << 8) - (1 << 7);                           /* A bit of noise makes it sound better */
            std::fwrite(&numberToBeWritten, 2, 1, wavefile);
        }
    }

    void finish()
    {
        std::fclose(wavefile);
        wavefile = 0;
    }

    static void notes2file(const std::string& filename, note_t* notes, int size)
    {
        WAV_writer writer;
        writer.create(filename);

        for (int s = 0; s < size; ++s)
        {
            const note_t& note = notes[s];
            writer.fill_note(
                note.duration,
                frequency(note.note, note.octave)
            );
        }

        writer.finish();
    }
};

//=============================================================================================================================================================================
//=============================================================================================================================================================================
//=============================================================================================================================================================================

/* array mix :: starting_index = 0, amount = 48 */
int nabornot[][6] = {
    {0, 1, 3, 5, 6, 8 },
    {0, 1, 3, 4, 6, 8 },
    {0, 1, 3, 5, 6, 9 },
    {0, 2, 3, 5, 6, 9 },
    {0, 3, 4, 6, 8, 11 },
    {0, 3, 4, 6, 7, 11 },
    {0, 1, 3, 5, 6, 9 },
    {0, 2, 3, 6, 7, 11 },
    {0, 2, 3, 6, 7, 10 },
    {0, 1, 3, 4, 6, 9 },
    {0, 1, 3, 5, 6, 9 },
    {0, 2, 3, 5, 6, 10 },
    {0, 1, 3, 5, 6, 10 },
    {0, 1, 3, 5, 7, 9 },
    {0, 2, 3, 5, 7, 8 },
    {0, 2, 3, 6, 7, 10 },
    {0, 3, 5, 7, 8, 11 },
    {0, 1, 3, 5, 7, 9 },
    {0, 2, 3, 7, 8, 11 },
    {0, 1, 3, 5, 7, 9 },
    {0, 1, 3, 4, 7, 10 },
    {0, 2, 3, 6, 7, 10 },
    {0, 2, 3, 5, 7, 11 },
    {0, 1, 4, 5, 7, 9 },
    {0, 2, 4, 5, 7, 8 },
    {0, 2, 4, 6, 7, 10 },
    {0, 3, 4, 6, 7, 9 },
    {0, 4, 5, 7, 8, 11 },
    {0, 1, 4, 5, 7, 9 },
    {0, 2, 4, 6, 7, 10 },
    {0, 2, 4, 7, 8, 11 },
    {0, 1, 4, 5, 7, 9 },
    {0, 2, 4, 6, 7, 10 },
    {0, 2, 4, 5, 7, 11 },
    {0, 2, 4, 6, 8, 9 },
    {0, 2, 4, 5, 8, 9 },
    {0, 3, 4, 6, 8, 9 },
    {0, 3, 4, 7, 8, 10 },
    {0, 3, 4, 6, 8, 10 },
    {0, 1, 4, 6, 8, 10 },
    {0, 1, 4, 6, 8, 9 },
    {0, 1, 4, 7, 8, 10 },
    {0, 2, 4, 7, 8, 10 },
    {0, 2, 4, 5, 8, 10 },
    {0, 1, 4, 5, 8, 10 },
    {0, 2, 4, 5, 8, 11 },
    {0, 3, 4, 6, 8, 11 },
    {0, 2, 4, 6, 8, 11 },
                                        /* array minmin :: starting_index = 48, amount = 8 */
    {0, 2, 3, 5, 6, 11 },
    {0, 1, 3, 4, 6, 10 },
    {0, 2, 3, 6, 8, 11 },
    {0, 1, 3, 6, 7, 10 },
    {0, 3, 5, 6, 8, 11 },
    {0, 3, 4, 6, 7, 10 },
    {0, 2, 3, 5, 6, 8 },
    {0, 1, 3, 4, 6, 7 },
                                                    /* array majmaj :: starting_index = 56, amount = 9 */
    {0, 3, 4, 7, 8, 11 },
    {0, 2, 4, 6, 8, 10 },
    {0, 1, 4, 5, 8, 9 },
    {0, 3, 4, 7, 8, 11 },
    {0, 2, 4, 6, 8, 10 },
    {0, 1, 4, 5, 8, 9 },
    {0, 3, 4, 7, 8, 11 },
    {0, 2, 4, 6, 8, 10 },
    {0, 1, 4, 5, 8, 9 },
                                                            /* array durmol :: starting_index = 65, amount = 12 */
    {0, 2, 4, 6, 7, 11 },
    {0, 1, 4, 5, 7, 10 },
    {0, 3, 4, 7, 8, 11 },
    {0, 1, 4, 6, 7, 9 },
    {0, 3, 4, 6, 7, 10 },
    {0, 2, 4, 5, 7, 9 },
    {0, 2, 3, 6, 7, 9 },
    {0, 1, 3, 5, 7, 8 },
    {0, 1, 3, 6, 7, 10 },
    {0, 1, 3, 4, 7, 9 },
    {0, 2, 3, 5, 7, 10 },
    {0, 3, 4, 7, 8, 11 },
                                                        /* array durdur :: starting_index = 77, amount = 5 */
    {0, 1, 4, 5, 7, 8 },
    {0, 3, 4, 6, 7, 11 },
    {0, 2, 4, 5, 7, 10 },
    {0, 1, 4, 6, 7, 10 },
    {0, 2, 4, 6, 7, 9 },
                                                        /* array molmol :: starting_index = 82, amount = 5 */
    {0, 1, 3, 4, 7, 8 },
    {0, 2, 3, 5, 7, 9 },
    {0, 1, 3, 6, 7, 9 },
    {0, 1, 3, 5, 7, 10 },
    {0, 2, 3, 6, 7, 11 },
};

const int KOLICHESTVO_NABOROFF_NOT = sizeof(nabornot) / sizeof(int[6]);
/*
void process_array(int* array, int N, std::string array_name)
{
    static int starting_index = 0;

    printf(" array %s :: starting_index = %d, amount = %d  \n", array_name.c_str(), starting_index, N);
    starting_index += N;

    for (int n = 0; n < N; ++n)
    {
        std::array<int, 6> notes;
        for (int j = 0; j < 6; ++j)
        {
            notes[j] = array[6 * n + j] % 12;
        }
        std::sort(notes.begin(), notes.end());

        printf("\t{");

        for (int j = 0; j < 5; ++j)
            printf("%d, ", notes[j]);

        printf("%d }, \n", notes[5]);
    }
}
*/

double dliniinot[11] = {1, 0.75, 0.5, 0.375, 0.25, 0.1875, 0.125, 0.09375, 0.0625, 0.046875, 0.03125};

//=============================================================================================================================================================================
//=============================================================================================================================================================================
//=============================================================================================================================================================================


std::random_device rd;                                                              //Wimajmaj be used to obtain a seed for the random number engine
std::mt19937 gen(rd());                                                             //Standard mersenne_twister_engine seeded with rd()


int min_index[] = { 0, 48, 56, 65, 77, 82};
int max_index[] = {47, 55, 64, 76, 81, 86};

int random(int a, int b)
{
    std::uniform_int_distribution<> distrib(a, b);
    return distrib(gen);
}

int random_2(int a)
{
    int u = 0;
    for (int c = 0; c < a; ++c)
    {
        u = u + random(-1, 1);
    }
    if (u < 0)
        u = -u;
    return u;
}

/* void print_note(int k)
{
    switch (k)
    {
        case 0:  std::cout << " C  " << std::endl; break;
        case 1:  std::cout << " cis" << std::endl; break;
        case 2:  std::cout << " D  " << std::endl; break;
        case 3:  std::cout << " dis" << std::endl; break;
        case 4:  std::cout << " E  " << std::endl; break;
        case 5:  std::cout << " F  " << std::endl; break;
        case 6:  std::cout << " fis" << std::endl; break;
        case 7:  std::cout << " G  " << std::endl; break;
        case 8:  std::cout << " gis" << std::endl; break;
        case 9:  std::cout << " A  " << std::endl; break;
        case 10: std::cout << " b  " << std::endl; break;
        case 11: std::cout << " H  " << std::endl; break;
    }
} */
//=============================================================================================================================================================================
//=============================================================================================================================================================================
//=============================================================================================================================================================================

int main()
{

//    process_array(&mix[0][0],    48, "mix");
//    process_array(&minmin[0][0], 8,  "minmin");
//    process_array(&majmaj[0][0], 9,  "majmaj");
//    process_array(&durmol[0][0], 12, "durmol");
//    process_array(&durdur[0][0], 5,  "durdur");
//    process_array(&molmol[0][0], 5,  "molmol");
//    return 0;
//
//    WAV_writer::notes2file("bethoveen.wav", moonlight_sonata, sonata_length);

start:
    int x, y, t;
    float sk, sd;

    std::cout << "kakije akordii hochesh v svojej melodije?" << std::endl;
    std::cout << "1: ostal'niije variantii (govno)" << std::endl;                            //durdur
    std::cout << "2: korotkij i korotkij" << std::endl;                        //molmol
    std::cout << "3: dlinnij i dlinnij\'" << std::endl;                          //durmol
    std::cout << "4: dur i mol" << std::endl;                  //minmin
    std::cout << "5: mol\' i mol\'" << std::endl;                    //majmaj
    std::cout << "6: dur i dur" << std::endl;        //mix
    std::cin >> x;
    while(1)
    {
        if((x != 1) && (x != 2) && (x != 3) && (x != 4) && (x != 5) && (x != 6))
        {
            std::cout << "Dubina napishii 1, 2, 3, 4, 5 ili 6!"<< std::endl;
            std::cin >> x;
        }
        else
            break;
    }

    std::cout << "Skol\'ko hochesh taktov?" << std::endl;
    std::cin >> t;
    while (t < 0)
    {
        std::cout << "Togda goni " << -t << " taktov, driistovzbzdisharegrapet. Skol\'ko hochesh taktov?" <<std::endl;
        std::cin >> t;
    }

    std::cout << "Kakaja budet samaja korotkaja nota? Jedinitza izmerenija: 1/32. Vozmozhnije tzisla: 1, 1.5, 2, 3, 4, 6, 8, 12, 16, 24 i 32." << std::endl;
    std::cin >> sk;

        if ((sk != 1) && (sk != 1.5) && (sk != 2) && (sk != 3) && (sk != 4) && (sk != 6) && (sk != 8) && (sk != 12) && (sk != 16) && (sk != 24) && (sk != 36))  sk = 4;
    std::cout << "Kakaja budet samaja dlinnaja nota? Jedinitza izmerenija: 1/32." << std::endl;
    std::cin >> sd;
        if ((sd != 1) && (sd != 1.5) && (sd != 2) && (sd != 3) && (sd != 4) && (sd != 6) && (sd != 8) && (sd != 12) && (sd != 16) && (sd != 24) && (sd != 32))  sd = 8;
        if (sk > sd) sd = 8;

    float ispdlinot[11];
    int q = 0;
    for (int a = 0; a < 11; ++a)
    {
        if ((dliniinot[a] <= (sd / 32.0f)) && (dliniinot[a] >= (sk / 32.0f)))
        {
            ispdlinot[q] = dliniinot[a];
            ++q;
        }
    }


    int i, f, j, s;
    std::cout <<"V kakoj primerno oktave dolzhnii biit\' notii?" <<std::endl;
    std::cin >> i;
    --i;

    std::cout << "Skol\'ko melodij nalabat\'?" <<std::endl;
    std::cin >> f;
    if (f < 0) f = 1;

    std::cout << "Hot\'it\'e li vii videt\' sinkopii v vashej melodije, ser? Net - 1; Da - 2; A tche eto? - 3." << std::endl;
    std::cin >> j;
    if (j == 3)
        {
            std::cout << "objasnenije; Tak che, ho ili net? Net - 1; Da - 2." <<std::endl;
            std::cin >> j;
        }


    std::cout << "Kakoj instrument? Pianino - 1; Gitara - 2; Bzdudka - 3; ..." <<std::endl;
    std::cin >> s;
    if ((s > 6) || (s < 1)) s = 1;


//=============================================================================================================================================================================
//=============================================================================================================================================================================
//=============================================================================================================================================================================
    WAV_writer writer;

    superstart:
    for (y = 0; y < f; ++y)
    {

        std::string filename = "output" + std::to_string(y) + ".wav";
        writer.create(filename);
        int g = random(0, 11);                                                                  //na skol'ko not uvelichivaestsa nabor not
        int r = random(min_index[x - 1], max_index[x - 1]);                                           //nomer nabora not
        float d = 0.0f;
        int e = random(6, 11);

        printf("vasha melodia #%d:\n", y + 1);

        while (t > d)
        {
            int c = 0;
            if (j == 1)
            {
                while (ispdlinot[c] > (1 - d + floor(d)))
                {
                    ++c;
                }
            }
            float h = ispdlinot[random(c, q - 1)];
            int o = i + floor(e / 6);
            int n = e % 6;
            float freq = frequency(nabornot[r][n], o);
            printf("{%f, %d, %d} :: %.5f \n", h, o, n, freq);     //kodovaja zapis'
            writer.fill_note(h, freq);
            int w = std::max(17 - e, e);
            int l = random_2(w);
            e = e + (2 * random(0, 1) - 1) * l;
            if (e < 0)
                e = e + 2 * l;
            if (e > 17)
                e = e - 2 * l;
            d = d + h;
        }
        writer.finish();
    }


    std::cout << "esh\'o ho? Da! :) -- 1; Da, :) i s takimi zhe parametrami! -- 2; Net! :( -- 3" << std::endl;
    int u;
    std::cin >> u;
    if (u == 1)
        goto start;
    if (u == 2)
        goto superstart;

    return 0;
}



