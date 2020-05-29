template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    
    ~ExpandableHashMap();// destructor; deletes all of the items in the hashmap
    
    void reset();// resets the hashmap back to 8 buckets, deletes all items
    
    int size() const; // return the number of associations in the hashmap
    
    // The associate method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    void associate(const KeyType& key, const ValueType& value);
    
    const ValueType* find(const KeyType& key) const;
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;
private:
    class pair{
    public:
        KeyType key;
        ValueType val;
        pair* next;
        pair(const KeyType& KEY, const ValueType& VALUE):key(KEY),val(VALUE),next(nullptr){};
        
    };
    const double maxLoadFac;
    int load;
    int numPair;
    int capacity;
    pair** pairArr;
    void expand(){
        pair** oldArr = pairArr;
        capacity*=2;
        load = 0;
        pairArr = new pair*[capacity];
        for (int i = 0; i< capacity; i++) pairArr[i] = nullptr;
        for (int i = 0; i< capacity/2; i++){
            pair* curr = oldArr[i];
            while (curr){
                associate(curr->key,curr->val);
                curr=curr->next;
            }
        }
        //delete oldArr
        for (int i = 0; i< capacity/2; i++){
            pair* curr = oldArr[i];
            while (curr){
                pair* temp = curr;
                delete curr;
                curr=temp->next;
            }
        }
        delete[] oldArr;
    }
};
template<typename KeyType, typename ValueType>

ExpandableHashMap<KeyType,ValueType>::ExpandableHashMap(double maximumLoadFactor)
:maxLoadFac(maximumLoadFactor),capacity(8),load(0),numPair(0)
{
    pairArr = new pair*[8];
    for (int i = 0; i< 8; i++) pairArr[i] = nullptr;
}
template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType,ValueType>::~ExpandableHashMap()
{
    // destructor; deletes all of the items in the hashmap
    for (int i = 0; i< capacity; i++){
        pair* curr = pairArr[i];
        while (curr){
            pair* temp = curr->next;
            delete curr;
            curr=temp;
        }
    }
    delete[] pairArr;
    
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>::reset(){
    // resets the hashmap back to 8 buckets, deletes all items
    for (int i = 0; i< capacity; i++){
        pair* curr = pairArr[i];
        while (curr){
            pair* temp = curr->next;
            delete curr;
            curr=temp;
        }
    }
    load=0;
    numPair=0;
    capacity=8;
    delete[] pairArr;
    pairArr = new pair*[8];
    for (int i = 0; i< 8; i++) pairArr[i] = nullptr;
}
template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType,ValueType>::size() const
{
    return numPair;
}
template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType,ValueType>::associate(const KeyType& key, const ValueType& value)
{
    // The associate method associates one item (key) with another (value).
    // If no association currently exists with that key, this method inserts
    // a new association into the hashmap with that key/value pair. If there is
    // already an association with that key in the hashmap, then the item
    // associated with that key is replaced by the second parameter (value).
    // Thus, the hashmap must contain no duplicate keys.
    unsigned int hasher(const KeyType& k);
    unsigned int h = hasher(key);
    pair* curr = pairArr[h%capacity];
    if(!curr){
        // when adding an item to your expandable hash map, its load factor would increase
        // above the maximum load specified during construction:
        if(load+1>capacity*maxLoadFac){
            expand();
            associate(key,value);
        }
        pairArr[h%capacity] = new pair(key,value);
        numPair++;
        load++;
    }else{
        pair* pre;
        while (curr){
            pre = curr;
            if (curr->key==key){
                curr->val = value;
                return;
            }
            curr = curr->next;
            
        }
        pre->next = new pair(key,value);
        numPair++;
    }
}
template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType,ValueType>::find(const KeyType& key) const
{
    // If no association exists with the given key, return nullptr; otherwise,
    // return a pointer to the value associated with that key. This pointer can be
    // used to examine that value, and if the hashmap is allowed to be modified, to
    // modify that value directly within the map (the second overload enables
    // this). Using a little C++ magic, we have implemented it in terms of the
    // first overload, which you must implement.
    unsigned int hasher(const KeyType& k);
    unsigned int h = hasher(key);
    pair* curr =pairArr[h%capacity];
    while (curr){
        if (curr->key==key)return &curr->val;
        curr=curr->next;
    }
    return nullptr;
}
