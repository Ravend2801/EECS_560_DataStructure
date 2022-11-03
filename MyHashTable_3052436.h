#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector_3052436.h"
#include "MyLinkedList_3052436.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
    // code begins
    return n % mersenne_prime;
    // code ends
}

// definition of the template hash function class
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
        long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;      // the base used for inflating each character
  public:
    long long univHash(const std::string& key, const long long table_size) const
    {
        long long hv = 0;
        for(size_t i = 0; i < key.length(); ++ i)
        {
            hv = param_base * hv + static_cast<long long>(key[i]);
        }
        hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
        hv = hv % table_size;
        return hv;
    }
};

// definition of the template hashed object class
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:

    KeyType key;
    ValueType value;

    HashedObj()
    {
        return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
        key(k),
        value(v)
    {
        return;
    }

    HashedObj(KeyType && k, ValueType && v) :
        key(std::move(k)),
        value(std::move(v))
    {
        return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
        return (key == rhs.key);
    }

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
        return !(*this == rhs);
    }

};

template <typename KeyType, typename ValueType>
class MyHashTable
{
  private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
        // code begins
        MyVector<bool> is_prime(n + 1);
        is_prime[0] = is_prime[1] = false;
        for(int i = 2; i <= n; i++){
          is_prime[i] = true;
        }

        for(int i = 2; i * i <= n; i++){
          if(is_prime[i] == true){
            for(int j = i * i; j <= n; j += i){
              is_prime[j] = false;
            }
          }
        }

        for(int i = 0; i < primes.size(); i++){
          primes.pop_back();
        }

        for(int i = 2; i <= n; i++){
          if(is_prime[i] == true && (i <= 3 || i > 16)){
            primes.push_back(i);
          }
        }

        // code ends
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
        // code begins
        size_t m = 2 * n;
        preCalPrimes(m);
        int low = 0, high = primes.size() - 1;
        while(primes[high] < n){
          m = m * 2;
          preCalPrimes(m);
          high = primes.size() - 1;
        }
        while(low < high){
          if(primes[low] == n){
            return primes[low];
          } else if(primes[low] > n){
            break;
          }
          low++;
        }
        return primes[low];
        // code ends
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(key, hash_table.size())];
        for(auto itr = whichList->begin(); itr != whichList->end(); itr++){
          HashedObj<KeyType, ValueType> temp = *itr;
          if(temp.key == key){
            return itr;
          }
        }
        return whichList->end();
        // code ends
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
        // code begins
        MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > oldhash_table = hash_table;

        hash_table.resize(nextPrime(new_size));

        for(int i = 0; i < hash_table.size(); i++){
          hash_table[i] = new MyLinkedList<HashedObj<KeyType, ValueType>>();
        }

        theSize = 0;
        for(int i = 0; i < oldhash_table.size(); i++){
          for(auto itr = oldhash_table[i]->begin(); itr != oldhash_table[i]->end(); itr++){
            HashedObj<KeyType, ValueType> temp = *itr;
            insert(std::move(temp));
          }
        }
        // code ends
    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
        size_t new_size = nextPrime(2 * hash_table.size());
        this->rehash(new_size);
        return;
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
        size_t new_size = nextPrime(ceil(hash_table.size() / 2));
        this->rehash(new_size);
        return;
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
        // code begins
        theSize = 0;
        for(int i = 0; i < init_size; i++){
          hash_table.push_back(new MyLinkedList<HashedObj<KeyType, ValueType>>());
        }
        // code ends
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
        // code begins
        for(auto & thisList : hash_table){
          thisList->clear();
        }
        for(int i = 0; i < hash_table.size(); i++){
          delete hash_table[i];
        }
        // code ends
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(key, hash_table.size())];
        auto itr = find(key);

        if(itr == whichList->end()){
          return false;
        }

        return true;
        // code ends
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(key, hash_table.size())];
        auto itr = find(key);

        if(itr == whichList->end()){
          return false;
        }
        data = *itr;

        return true;
        // code ends
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(x.key, hash_table.size())];
        if(find(x.key) != whichList->end()){
          return false;
        }
        whichList->push_back(x);

        if(++theSize > hash_table.size() / 2){
          doubleTable();
        }
        return true;
        // code ends
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(x.key, hash_table.size())];
        if(find(x.key) != whichList->end()){
          return false;
        }
        whichList->push_back(std::move(x));

        if(++theSize > hash_table.size() / 2){
          doubleTable();
        }
        return true;
        // code ends
    }

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
        // code begins
        HashFunc<KeyType> myhash;
        auto& whichList = hash_table[myhash.univHash(key, hash_table.size())];
        auto itr = find(key);

        if(itr == whichList->end()){
          return false;
        }

        whichList->erase(itr);
        if((--theSize) * 8 < hash_table.size()){
          halveTable();
        }
        return true;
        // code ends
    }

    // returns the number of data elements stored in the hash table
    size_t size()
    {
        // code begins
        return theSize;
        // code ends
    }

    // returns the capacity of the hash table
    size_t capacity()
    {
        // code begins
        return hash_table.size();
        // code ends
    }

};


#endif // __MYHASHTABLE_H__
