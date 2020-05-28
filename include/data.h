#ifndef DATA_INCLUDED
#define DATA_INCLUDED
//-----------------------------------------------------------------------------
struct BYTE_DATA
{
private:
    BYTE prevValue;
public:
    BYTE value;
    BYTE updated;
    BYTE_DATA()
    {
        value = 0xFF;
        prevValue = 0xFF;
        updated = 0;
    }
    BYTE_DATA(BYTE new_value)
    {
        value = new_value;
    }
    void operator=(BYTE new_value)
    {
        value = new_value;
        updated = 1;
    }
    bool changed()
    {
        register bool tmp = (value != prevValue);
        if(tmp)
        {
            prevValue = value;
        }
        return tmp;
    }
    operator bool()
    {
        return (bool) value;
    }
};
//-----------------------------------------------------------------------------
struct WORD_DATA
{
private:
    WORD prevValue;
public:
    WORD value;
    BYTE updated;
    WORD_DATA()
    {
        value = 0xFFFF;
        prevValue = 0xFFFF;
        updated = 0;
    }
    WORD_DATA(WORD new_value)
    {
        value = new_value;
    }
    void operator=(WORD new_value)
    {
        value = new_value;
        updated = 1;
    }
    void operator++(int)
    {
        value++;
    }
    bool changed()
    {
        register bool tmp = (value != prevValue);
        if(tmp)
        {
            prevValue = value;
        }
        return tmp;
    }
    operator bool()
    {
        return (bool) value;
    }
};
//-----------------------------------------------------------------------------
#endif // DATA_H_INCLUDED
