#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>

using namespace std;

class DH
{
public:
    int choice;
    int p, g, xa, xb;
    int ya, yb;
    int ka, kb;

    void printVec(vector <int> const &pr) {
        std::cout << "= ";

        for(int i=0; i < pr.size(); i++)
            std::cout << pr.at(i) << ' ';
    }

    int powmod(int a,int b,int p)
    {
        //Base cases
        if(b==0)
            return 1;

        if(b==1)
            return a;

        //Finding answer for power b/2
        int ans = powmod(a,(b/2),p);
        ans = (ans*ans)%p;

        if(b&1)
            ans = (ans*a)%p;

        return ans;
    }
     int squareAndMultiply(int a, int b, int n){
         int result = 1;
         while (b > 0)
         {
             if (b % 2 == 1)
                 result = (result * a) % n;
             b = b >> 1; //Binary Right Shift Operator.
                        // The left operands value is moved right by the number of bits specified by the right operand.
             a = (a * a) % n;
         }
         return result;
    }
    bool primeNumber(int n){
        bool flag = true;
        for(int i = 2; i <= n / 2; i++) {
            if(n % i == 0) {
                flag = false;
                break;
            }
        }
        return flag;
    }

    int random(){
        //Map to store when repetitive number come
        map<int,int> m;
        vector<int> prime;
        vector<int> pr;

        int ya,yb,ka,kb,xa,xb;
        xa = rand() % 100;
        xb = rand() % 100;

        srand(time(0));
        int p = rand() % 100; //random number between 1 - 100
        bool isPrime = true;

        for (int n = 2; n < p; n++) {
            // isPrime will be true for prime numbers
            isPrime = primeNumber(n);

            if (isPrime == true) {
                prime.push_back(n);
                //cout << "p =. " << n << "\n";
            }
        }
        int primeElement = prime[rand() % prime.size()]; //random prime number limited to 100
        cout << "\np = " << primeElement << endl;
        if(primeElement <= 2){
            cout << "Prime number shouldn't less than 3, Please run it again!! \n";
            EXIT_FAILURE;
        }else {
            //Find primitive root (g) //Outer loop iterate over g
            for (int i = 2; i < primeElement; i++) {
                m.clear();

                //Inner loop iterate over its multiplicative order
                for (int j = 1; j <= primeElement; j++) {
                    int x = squareAndMultiply(i, j, primeElement);
                    if (m.find(x) == m.end()) {
                        m[x]++;
                    } else {
                        break;
                    }
                }
                //Checking with num-1
                //Since phi(n) = n-1 for primes
                if (primeElement - 1 == m.size()) {
                    //cout << "g =."<< i << " ";
                    pr.push_back(i);
                }
            }
        }
        cout << "primitive root of " << primeElement;
        printVec(pr);
        int prElement = pr[rand() % pr.size()]; //generated random number of primitive root
        cout << "\nrandomly selected primitive root, g = " << prElement << endl;

        if(prElement > 0) {
            cout << "\nxa = " << xa << endl;
            cout << "xb = " << xb << endl;

            ya = squareAndMultiply(prElement, xa, primeElement);
            yb = squareAndMultiply(prElement, xb, primeElement);
            cout << "\nAlice's private key, YA = " << ya << endl;
            cout << "Bob's private key, YB = " << yb << endl;


            ka = squareAndMultiply(yb, xa, primeElement); //Alice
            kb = squareAndMultiply(ya, xb, primeElement); //Bob
            cout << "\nAlice's : K = " << ka << endl;
            cout << "Bob's : K = " << kb << endl;
        }else{
            cout << "g must be greater than 0, Run it again something went wrong..!!\n";
            EXIT_FAILURE;
        }

        return 0;
    }
};

int main() {
        DH d;
        map<int,int> m;
        vector<int> pr;
        cout << "\nPlease choose 1-3 of options below\n\n";
        cout << "1. Manual\n" << "2. Random\n" << "3. Square and Multiply Algorithm\n" << "4. Check prime number" << endl;
        cin >> d.choice;
        if (d.choice == 1) {
            cout << "Enter the values of p and g : " << endl;
            cout << "\nEnter p(Prime number): ";
            cin >> d.p; // must be prime number
            if(d.primeNumber(d.p) == false){
                cout << "\np must be a prime number!!\n";
                cout << "\nStart program again..\n";
                exit(1); //EXIT FAILURE
            }
            //Find primitive root (g) //Outer loop iterate over g
            for (int i = 2; i < d.p; i++) {
                m.clear();

                //Inner loop iterate over its multiplicative order
                for (int j = 1; j <= d.p; j++) {
                    int x = d.squareAndMultiply(i, j, d.p);
                    if (m.find(x) == m.end()) {
                        m[x]++;
                    } else {
                        break;
                    }
                }
                //Checking with num-1
                //Since phi(n) = n-1 for primes
                if (d.p - 1 == m.size()) {
                    //cout << "g =."<< i << " ";
                    pr.push_back(i);
                }
            }
            cout << "From that p we have, primitive root ";
            d.printVec(pr);
            cout << endl;
            cout << "Enter g(Primitive root from above): "; // primitively root
            cin >> d.g;
            if(count(pr.begin(), pr.end(), d.g)) {
                cout << "Enter xa : ";
                cin >> d.xa;
                cout << "Enter xb : ";
                cin >> d.xb;
                cout << endl;

                d.ya = d.powmod(d.g, d.xa, d.p);
                d.yb = d.powmod(d.g, d.xb, d.p);
                cout << "Alice's private key, YA = " << d.ya << endl;
                cout << "Bob's private key, YB = " << d.yb << endl;
                cout << endl;


                d.ka = d.powmod(d.yb, d.xa, d.p); //Alice
                d.kb = d.powmod(d.ya, d.xb, d.p); //Bob
                cout << "Alice's : K = " << d.ka << endl;
                cout << "Bob's : K = " << d.kb << endl;
            }else{
                cout << "Its not primitive root of that p\n";
                cout << "Run program again..\n";
                exit(1); // EXIT FAILURE
            }

        }
        if (d.choice == 2) {
                srand(time(0));// set the starting point for producing a series of pseudo-random integers.
                d.random();
        }
        if(d.choice == 3){

            int a, b, n;
            int output;
            cout << "Input your a, b, n, to find a^b mod n\n";
            cout << "a = ";
            cin >> a;
            cout << "b = ";
            cin >> b;
            cout << "n = ";
            cin >> n;

            output = d.squareAndMultiply(a, b, n);
            cout << "a^b mod n = " << output << endl;
        }
        if(d.choice == 4){
            int p;
            cout << "Input your number to check, that is a prime number or not\n";
            cin >> p;
            if(d.primeNumber(p) == false){
                cout << "Its not a prime number!!\n";
            }else{
                cout << "That's a prime number\n";
            }
        }
        return 0;
}
