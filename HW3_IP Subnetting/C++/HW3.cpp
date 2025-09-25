#include <iostream>
#include <cmath>
#include <string>

#define SIZE 4

using namespace std;

class SubNetting
{
private:
    string IP_address;
    int subnet_size;
    char netType;
    int IP_range = 0;
    int original_subnet_size = 0;
    int IP_array[SIZE] = {-1, -1, -1, -1}; // SIZE = 4
    bool getDefaultIP();
    void classficateIP();
    int countMaxIP();
    int toBinary(int n);
    void process_address(int *address, int add_range);
    void printIP(int address[SIZE]);

public:
    SubNetting(string IP_address, int num_subnets)
    {
        this->IP_address = IP_address;
        // initailize IP_array
        string temp_arr[4] = {""};
        int temp = 0;
        for (int index = 0; index < IP_address.length(); index++)
        {
            if (IP_address[index] == '.')
            {
                temp++;
                continue;
            }
            else
            {
                temp_arr[temp] += IP_address[index];
            }
        }
        // string to integer
        for (int i = 0; i < SIZE; i++)
        {
            IP_array[i] = stoi(temp_arr[i]);
        }
        // Validate IP
        for (int i = 0; i < SIZE; i++)
        {
            if (IP_array[i] < 0 || IP_array[i] > 255)
            {
                cout << "Invalid IP address" << endl;
                exit(1);
            }
        }
        // Classify IP
        this->classficateIP();
        // Validate subnet size
        this->subnet_size = num_subnets;
        this->original_subnet_size = this->subnet_size;
        if (this->subnet_size < 0 || this->subnet_size > 32)
        {
            cout << "Invalid subnet size" << endl;
            exit(1);
        }

        for (int i : IP_array)
            cout << i << " ";
        cout << endl;
    }

    void getIP()
    { // just for debugging
        // return this->IP_address;
        for (int i = 0; i < SIZE; i++)
        {
            if (i == 3)
                cout << this->IP_array[i] << endl;
            else
                cout << this->IP_array[i] << ".";
        }
    }

    void outputAll();
};

void SubNetting::classficateIP()
{
    if (this->IP_array[0] >= 0 && this->IP_array[0] <= 127)
        this->netType = 'A';
    else if (this->IP_array[0] >= 128 && this->IP_array[0] <= 191)
        this->netType = 'B';
    else if (this->IP_array[0] >= 192 && this->IP_array[0] <= 223)
        this->netType = 'C';
    else
        this->netType = 'D'; // should not happen
}

int SubNetting::countMaxIP()
{
    int max_IP = 0;
    if (this->netType == 'A')
        max_IP = pow(2, 24);
    else if (this->netType == 'B')
        max_IP = pow(2, 16);
    else if (this->netType == 'C')
        max_IP = pow(2, 8);
    else
        // exception("Invalid netType, IP is not valid"); // should never happen
        printf("Invalid netType, IP is not valid");
    cout << "maxIP: " << max_IP << endl;

    this->subnet_size = pow(2, ceil(log2(subnet_size)));
    cout << "subnet range: " << max_IP / subnet_size << endl;

    this->IP_range = max_IP / subnet_size;
    return this->IP_range;
}

// int SubNetting::toBinary(int n)
// {
//     string r;
//     while (n != 0)
//     {
//         r += (n % 2 == 0 ? "0" : "1");
//         n /= 2;
//     }
//     return stoi(r);
// }

bool SubNetting::getDefaultIP()
{

    switch (this->netType) // [192,168,50,2]
    {
    case 'A':
        this->IP_array[1] &= 0x00000000;
    case 'B':
        this->IP_array[2] &= 0x00000000;
    case 'C':
        this->IP_array[3] &= 0;
    }
    cout << "first IP: ";
    for (int i = 0; i < SIZE; i++)
    {
        if (i == 3)
            cout << this->IP_array[i] << endl;
        else
            cout << this->IP_array[i] << ".";
    }
}

void SubNetting::process_address(int *address, int add_range)
{
    address[3] += add_range;
    for (int i = 1; i < SIZE; i++)
    {
        address[SIZE - i - 1] += address[SIZE - i] / 256;
        address[SIZE - i] %= 256;
    }
}

void SubNetting::printIP(int address[SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        if (i == 3)
            cout << address[i] << endl;
        else
            cout << address[i] << ".";
    }
}

void SubNetting::outputAll()
{
    int max_IP = countMaxIP();
    if (max_IP < 4)
    {
        cout << "Not enough IPs to split" << endl;
        exit(0);
    }
    cout << "final check: ";
    printIP(IP_array);
    int output_arr[4] = {-1};

    getDefaultIP();
    printIP(IP_array);

    for (int i = 0; i < SIZE; i++)
    {
        output_arr[i] = this->IP_array[i];
    }

    for (int i = 0; i < this->subnet_size; i++)
    {   
        if (i == this->original_subnet_size)
        {
            return;
        }
        
        cout << "----------------------------------------" << endl;
        cout << "Subnet: " << i + 1 << endl;
        cout << "Network ID: ";
        printIP(output_arr);
        cout << "IP range: " << endl;
        printIP(output_arr);
        cout << " ~ " << endl;
        process_address(output_arr, IP_range - 3);
        printIP(output_arr);
        process_address(output_arr, 1);
        cout << "Gateway address: ";
        printIP(output_arr);
        process_address(output_arr, 1);
        cout << "Broadcast address: ";
        printIP(output_arr);
        process_address(output_arr, 1);
        cout << endl;
        
    }
    cout << "----------------------------------------" << endl;
}

int main()
{
    string IP_address;
    int num_subnets;
    cout << "Enter IP address: ";
    cin >> IP_address;
    cout << "Enter number of subnets: ";
    cin >> num_subnets;
    SubNetting subnet(IP_address, num_subnets);
    subnet.outputAll();
    return 0;
}
