#include <iostream>
#include <vector>

const int M_MAX = 65536;

int LCG_function(const int x, const int a_coefficient, const int c_increment, const int m_modulo) {
    return (a_coefficient * x + c_increment) % m_modulo;
}

// нахождение мультипликативного обратного
int modular_inverse(int a, int m) {
    int t, nt, r, nr, q, tmp;
    t = 0;
    nt = 1;
    r = m;
    nr = a % m;
    while (nr != 0) {
        q = r / nr;
        tmp = nt;
        nt = t - q * nt;
        t = tmp;
        tmp = nr;
        nr = r - q * nr;
        r = tmp;
    }
    if (r > 1) return -1;
    if (t < 0) t += m;
    return t;
}

struct coeff {
    int a;
    int c;
    int m;
    int x4;
};

std::vector<coeff> bruteforce(const int x0, const int x1, const int x2, const int x3) {
    std::vector<coeff> c_vec;
    // так ни один коэффициент не известен, начнем с перебора m
    for (int m = 2; m <= M_MAX; ++m) {
        // перейдем к последовательсти разностей,
        // чтобы избавиться от неизвестного коэффициента c
        int diff0 = (x1 - x0);
        int diff1 = (x2 - x1);
        int diff2 = (x3 - x2);

        if (diff0 < 0) {
            diff0 += m;
        }
        if (diff1 < 0) {
            diff1 += m;
        }
        if (diff2 < 0) {
            diff2 += m;
        }
        
        int inv_diff0 = modular_inverse(diff0, m);
        // если обратное по модулю не нашлось пропускаем этот m
        if (inv_diff0 < 0) continue;
        int a = (diff1 * inv_diff0) % m;

        // проверяем последовательность разностей с найденым a и подобранным m
        int my_diff1 = (a * diff0) % m;
        int my_diff2 = (a * diff1) % m;

        if (my_diff1 == diff1 && my_diff2 == diff2) {
            // выразим неизвестный коэффициент c
            int c = (x1 - a * x0) % m;
            if (c < 0) {
                c += m;
            }
            int x4 = LCG_function(x3, a, c, m);
            coeff tmp = {a, c, m, x4};
            c_vec.push_back(tmp);
            std::cout << "coefficient a = " << a << std::endl;
            std::cout << "increment c = " << c << std::endl;
            std::cout << "modulo m = " << m << std::endl;
            std::cout << "5th number = " << x4 << std::endl;
            std::cout << std::endl;
        }
    }
    if (c_vec.size() == 0) {
        std::cerr << "5th number not found" << std::endl;
    }
    return c_vec;
}
// являются ли числа взаимно простыми
int gcd(int x, int y) {
    return y ? gcd(y, x % y) : x;
}
// наиболее вероятные коэффициенты и 5-й элемент находятся исходя из того, 
// что m и c должны быть взаимно простыми 
void select_coeff(std::vector<coeff> c_vec) {
    if (c_vec.size() == 0) {
        return;
    }
    std::vector<coeff> best_coeff;
    for (auto & it : c_vec) {
        if (gcd(it.c, it.m) == 1) {
            coeff possible_coeff = {it.a, it.c, it.m, it.x4};
            best_coeff.push_back(possible_coeff);
        }
    }
    if (!best_coeff.empty()) {
        for (auto & it : best_coeff) {
            std::cout << "Most possible 5ths numbers = " << it.x4 << ", a = " << it.a <<
                      " c = " << it.c << " m = " << it.m << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        std::cerr << "Required 4 input numbers!" << std::endl;
        return EXIT_FAILURE;
    }

    int numbers[4];
    for (int i = 1; i < argc; ++i) {
    	try{
    
        numbers[i - 1] = std::stoi(argv[i]);
        }
    	catch(std::invalid_argument const&ex){
    	std::cerr << "Invalid argument! "<< ex.what()<< std::endl;
    	return EXIT_FAILURE;
    	}
    	
    }

    int x0 = numbers[0];
    int x1 = numbers[1];
    int x2 = numbers[2];
    int x3 = numbers[3];

    std::vector<coeff> x4 = bruteforce(x0, x1, x2, x3);
    select_coeff(x4);

    return 0;
}
