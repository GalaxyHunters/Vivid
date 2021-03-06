
#ifndef COLORMAP
#define COLORMAP

#include <vector>
#include <iostream>
#include "ModelBuilder/Point.h"

// TODO: redesign color scheme

using namespace std;

class color_t
{
public:
    double R;
    double G;
    double B;

	inline color_t(double R, double G, double B) : R(R), G(G), B(B) {}
//	inline color_t(int8_t R, int8_t G, int8_t B) : R(R), G(G), B(B) {}
};


const vector<color_t> ClmPlasma{color_t(0.050383, 0.029803, 0.52797),
                                 color_t(0.086222, 0.026125, 0.54266),
                                 color_t(0.11512, 0.023556, 0.55547),
                                 color_t(0.14861, 0.021154, 0.57056),
                                 color_t(0.17157, 0.019706, 0.58081),
                                 color_t(0.20045, 0.017902, 0.59336),
                                 color_t(0.2212, 0.016497, 0.60208),
                                 color_t(0.24803, 0.014439, 0.61287),
                                 color_t(0.2677, 0.012716, 0.62035),
                                 color_t(0.29348, 0.010213, 0.62949),
                                 color_t(0.31254, 0.008239, 0.6357),
                                 color_t(0.33768, 0.005618, 0.64305),
                                 color_t(0.35636, 0.003798, 0.64781),
                                 color_t(0.3749, 0.002245, 0.65188),
                                 color_t(0.39941, 0.000859, 0.65613),
                                 color_t(0.41764, 0.000564, 0.65839),
                                 color_t(0.44173, 0.00154, 0.66007),
                                 color_t(0.45962, 0.003574, 0.66028),
                                 color_t(0.48321, 0.00846, 0.65909),
                                 color_t(0.50068, 0.014055, 0.65709),
                                 color_t(0.52363, 0.024532, 0.6529),
                                 color_t(0.54057, 0.03495, 0.64864),
                                 color_t(0.56274, 0.051545, 0.64151),
                                 color_t(0.57903, 0.064296, 0.63513),
                                 color_t(0.60027, 0.081516, 0.62534),
                                 color_t(0.61581, 0.094564, 0.61714),
                                 color_t(0.63102, 0.1077, 0.60829),
                                 color_t(0.65075, 0.12531, 0.59562),
                                 color_t(0.66513, 0.13857, 0.58558),
                                 color_t(0.68376, 0.15628, 0.57166),
                                 color_t(0.69732, 0.16957, 0.56092),
                                 color_t(0.71488, 0.1873, 0.54634),
                                 color_t(0.72767, 0.20059, 0.53529),
                                 color_t(0.74423, 0.21829, 0.52052),
                                 color_t(0.7563, 0.23156, 0.50947),
                                 color_t(0.77196, 0.24924, 0.49481),
                                 color_t(0.78338, 0.2625, 0.48392),
                                 color_t(0.79822, 0.2802, 0.46954),
                                 color_t(0.80905, 0.29349, 0.45887),
                                 color_t(0.81965, 0.30681, 0.44831),
                                 color_t(0.83342, 0.32464, 0.43437),
                                 color_t(0.84348, 0.33806, 0.42401),
                                 color_t(0.85655, 0.35607, 0.41032),
                                 color_t(0.86608, 0.36966, 0.40013),
                                 color_t(0.87842, 0.38793, 0.3866),
                                 color_t(0.8874, 0.40176, 0.37649),
                                 color_t(0.89898, 0.42039, 0.36305),
                                 color_t(0.90736, 0.43452, 0.35297),
                                 color_t(0.91811, 0.4536, 0.33953),
                                 color_t(0.92583, 0.4681, 0.32943),
                                 color_t(0.93323, 0.48278, 0.31933),
                                 color_t(0.9426, 0.50264, 0.30582),
                                 color_t(0.94922, 0.51776, 0.29566),
                                 color_t(0.95747, 0.53825, 0.2821),
                                 color_t(0.9632, 0.55387, 0.27191),
                                 color_t(0.9702, 0.57503, 0.25833),
                                 color_t(0.97495, 0.59117, 0.24815),
                                 color_t(0.98056, 0.61304, 0.23465),
                                 color_t(0.9842, 0.62972, 0.22459),
                                 color_t(0.98826, 0.65233, 0.21136),
                                 color_t(0.99068, 0.66956, 0.20164),
                                 color_t(0.99303, 0.69291, 0.18908),
                                 color_t(0.9941, 0.7107, 0.1801),
                                 color_t(0.99455, 0.72873, 0.17162),
                                 color_t(0.99414, 0.75314, 0.1614),
                                 color_t(0.99303, 0.77172, 0.15481),
                                 color_t(0.99044, 0.79686, 0.14787),
                                 color_t(0.98762, 0.81598, 0.14436),
                                 color_t(0.98265, 0.84181, 0.1423),
                                 color_t(0.97799, 0.86143, 0.14281),
                                 color_t(0.97053, 0.8879, 0.14592),
                                 color_t(0.96402, 0.90795, 0.14937),
                                 color_t(0.95429, 0.93491, 0.15292),
                                 color_t(0.9466, 0.95519, 0.15033),
                                 color_t(0.94002, 0.97516, 0.13133)
};

inline static int GetColorIndex( double aVal) // float aColorMap ----- when we add support for more color maps an option for choice between them will be added
{
    if (ClmPlasma.empty()){ // TODO: not suppose to happen fix later
        return 0;
    }
    return floor (aVal * (double(ClmPlasma.size())-1)); //returns the index of the color in ClmPlasma
}

inline static color_t GetColor(double aVal) //float aColorMap ----- when we add support for more color maps an option for choice between them will be added
{
    return ClmPlasma[GetColorIndex(aVal)];
}


inline static vector<unsigned char> GetColorTexture()//float aColorMap
{
    // TODO 255 is a magic num. take char.max or something
    vector<unsigned char> texture;
     for(auto it = ClmPlasma.begin(); it != ClmPlasma.end(); it++){
        texture.push_back((unsigned char) ((*it).R*255) );
        texture.push_back((unsigned char) ((*it).G*255) );
        texture.push_back((unsigned char) ((*it).B*255) );
        texture.push_back((unsigned char) (255));
    }
    texture.resize(texture.size());
    return texture;
}

#endif