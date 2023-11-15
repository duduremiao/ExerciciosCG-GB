#include <noise.h>
#include <noiseutils.h>
#include <stdio.h>

int main() {
    int width = 400;
    int height = 400;
    double scale = 50.0;
    module::Perlin perlinModule;
    utils::NoiseMap heightMap;
    utils::NoiseMapBuilderPlane heightMapBuilder;
    utils::Image image;

    //Gerar 4 mapas "vizinhos" com diferentes parâmetros de bounds
    const int bounds[] = { 0, 100, 100, 200, 200, 300, 300, 400 };
    utils::RendererImage renderer;
    for (int i = 0; i < 4; i++) {
        heightMapBuilder.SetSourceModule(perlinModule);
        heightMapBuilder.SetDestNoiseMap(heightMap);
        heightMapBuilder.SetBounds(bounds[i * 2], bounds[i * 2 + 1], 0.0, 1.0);
        heightMapBuilder.Build();

        renderer.SetSourceNoiseMap(heightMap);
        renderer.SetDestImage(image);
        renderer.Render();

        char filename[20];
        snprintf(filename, sizeof(filename), "mapa_vizinho%d.png", i + 1);
        utils::WriterBMP writer;
        writer.SetSourceImage(image);
        writer.SetDestFilename(filename);
        writer.WriteDestFile();
    }

    //Explorar números diferentes de oitavas, frequência e persistência
    const int numOctaves[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const double baseFrequency = 1.0;
    const double persistence = 0.5;
    for (int i = 0; i < 8; i++) {
        perlinModule.SetOctaveCount(numOctaves[i]);
        perlinModule.SetFrequency(baseFrequency * pow(2, numOctaves[i] - 1));
        perlinModule.SetPersistence(pow(persistence, numOctaves[i]));

        heightMapBuilder.SetSourceModule(perlinModule);
        heightMapBuilder.SetDestNoiseMap(heightMap);
        heightMapBuilder.SetBounds(0.0, 400.0, 0.0, 400.0);
        heightMapBuilder.Build();

        renderer.SetSourceNoiseMap(heightMap);
        renderer.SetDestImage(image);
        renderer.Render();

        char filename[20];
        snprintf(filename, sizeof(filename), "oitavas%d.png", numOctaves[i]);
        utils::WriterBMP writer;
        writer.SetSourceImage(image);
        writer.SetDestFilename(filename);
        writer.WriteDestFile();
    }

    //Criar pelo menos 2 diferentes mapeamentos de biomas
    module::Curve curveModule;
    curveModule.SetSourceModule(0, perlinModule);
    curveModule.AddControlPoint(-1.0, -1.0);
    curveModule.AddControlPoint(-0.25, -0.25);
    curveModule.AddControlPoint(0.25, 0.25);
    curveModule.AddControlPoint(1.0, 1.0);
    perlinModule.SetPersistence(0.5);

    for (int i = 0; i < 2; i++) {
        heightMapBuilder.SetSourceModule(curveModule);
        heightMapBuilder.SetDestNoiseMap(heightMap);
        heightMapBuilder.SetBounds(0.0, 400.0, 0.0, 400.0);
        heightMapBuilder.Build();

        renderer.SetSourceNoiseMap(heightMap);
        renderer.SetDestImage(image);
        renderer.Render();

        char filename[20];
        snprintf(filename, sizeof(filename), "bioma%d.png", i + 1);
        utils::WriterBMP writer;
        writer.SetSourceImage(image);
        writer.SetDestFilename(filename);
        writer.WriteDestFile();
    }

    return 0;
}
