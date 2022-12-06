#ifndef FIR_FILTER_H
#define FIR_FILTER_H

#include <cmath>
#include <tuple>
#include <x86intrin.h>
#include <iostream>
#include <algorithm>
#include <immintrin.h>

// частота среза
const int pb = 15;

std::tuple<float*, float*> generate_in_signal(int in_size){
    // частота дискретизации
    int samp_freq = 1000;
    float time_stamp = 1.f/samp_freq;

    float *in_signals = new float[in_size];
    float *t = new float[in_size];
    float low = 5;
    float high = 50;

    float a = -0.1;
    float b = 0.01;

    // шумовой синусоидальный сигнал
    for(int i =0; i<in_size;++i){

        t[i]=time_stamp*i;
        float noise =(a+ static_cast<float>(rand())/RAND_MAX * (b-a));
        in_signals[i] = noise+(std::sin(M_PI*low*t[i]))+0.2*std::sin(M_PI*high*t[i]);
    }

    return std::make_tuple(t,in_signals);
}

float *FIR_filter(const float *input,const int in_size,const float samp_freq, int filter_length){
    unsigned int start_time =  clock();
    float *output = new float[in_size];
    int arr_size = in_size;

    // импульсная характеристика фильтра
    float h[filter_length];

    // идеальная импульсная характеристика
    float h_id[filter_length];
    // весовая функция
    float w[filter_length];

    // нормализованая частота перехода
    float ft = pb/samp_freq;

    // расчет импульсной характеристики фильтра
    for(int i =0;i<filter_length;i++){
        if (i==0) {
            h_id[i] = 2*ft;
        }
        else{
            h_id[i] = std::sin(2*M_PI*ft*i )/(M_PI*i);
        }
        // оконная функция Блэкмана
        w[i] = 0.42+0.5*std::cos((2*M_PI*i)/(filter_length-1)) + 0.08+std::cos((4*M_PI*i)/(filter_length-1));
        h[i] = h_id[i]*w[i];
    }

    // нормализация импульсной характеристики
    float sum=0;
    for(int i =0;i<filter_length;++i){
        sum+=h[i];
    }
    for(int i=0;i<filter_length;++i){
        h[i]/=sum;
    }


    // применение фильтра к входному сигналу
    for(int i=0;i<arr_size;++i){
        for(int j=0;j<filter_length-1;++j){
            if(i-j>0)
                output[i]+=h[j]*input[i-j];

        }
    }

    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout<<"FIR filter time: "<< search_time<<"ms"<<std::endl;
    return output;
};



constexpr auto AVX_FLOAT_COUNT = 8u;

float *FIR_filter_SIMD(const float *input,const int in_size,const float samp_freq, int filter_length){
    unsigned int start_time =  clock();

    int arr_size = in_size;
    float *output = new float[arr_size];
    // импульсная характеристика фильтра
    float h[filter_length];

    // идеальная импульсная характеристика
    float h_id[filter_length];
    // весовая функция
    float w[filter_length];

    // нормализованая частота перехода
    float ft = pb/samp_freq;

    // расчет импульсной характеристики фильтра
    for(int i =0;i<filter_length;i++){
        if (i==0) {
            h_id[i] = 2*ft;
        }
        else{
            h_id[i] = std::sin(2*M_PI*ft*i )/(M_PI*i);
        }
        // оконная функция Блэкмана
        w[i] = 0.42+0.5*std::cos((2*M_PI*i)/(filter_length-1)) + 0.08+std::cos((4*M_PI*i)/(filter_length-1));
        h[i] = h_id[i]*w[i];
    }

    // нормализация импульсной характеристики
    float sum=0;
    for(int i =0;i<filter_length;++i){
        sum+=h[i];
    }
    for(int i=0;i<filter_length;++i){
        h[i]/=sum;
    }

    // применение фильтра к входному сигналу
    for(int i=0;i<arr_size;i+=AVX_FLOAT_COUNT){
        auto outChunk = _mm256_setzero_ps();
        for(int j =0;j<filter_length;j++)
        {
            if(i-j>0)
            {
                auto inChunk = _mm256_loadu_ps(input + i - j);
                auto hChunk = _mm256_set1_ps(h[j]);

                auto temp = _mm256_mul_ps(inChunk, hChunk);
                // Add to the accumulators
                outChunk = _mm256_add_ps(outChunk, temp);
            }

        }
            _mm256_storeu_ps(output + i, outChunk);
    }
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    std::cout<<"FIR filter with SIMD time: "<< search_time<<"ms"<<std::endl;
    return output;
};


#endif // FIR_FILTER_H
