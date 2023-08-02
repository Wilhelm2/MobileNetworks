//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/timeoutHandoff.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include "timeoutHandoff_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace {
template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)(static_cast<const omnetpp::cObject *>(t));
}

template <class T> inline
typename std::enable_if<std::is_polymorphic<T>::value && !std::is_base_of<omnetpp::cObject,T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)dynamic_cast<const void *>(t);
}

template <class T> inline
typename std::enable_if<!std::is_polymorphic<T>::value, void *>::type
toVoidPtr(T* t)
{
    return (void *)static_cast<const void *>(t);
}

}


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule to generate operator<< for shared_ptr<T>
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const std::shared_ptr<T>& t) { return out << t.get(); }

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');

    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(TimeoutHandoff)

TimeoutHandoff::TimeoutHandoff(const char *name, short kind) : ::AlgoTimeout(name, kind)
{
}

TimeoutHandoff::TimeoutHandoff(const TimeoutHandoff& other) : ::AlgoTimeout(other)
{
    copy(other);
}

TimeoutHandoff::~TimeoutHandoff()
{
}

TimeoutHandoff& TimeoutHandoff::operator=(const TimeoutHandoff& other)
{
    if (this == &other) return *this;
    ::AlgoTimeout::operator=(other);
    copy(other);
    return *this;
}

void TimeoutHandoff::copy(const TimeoutHandoff& other)
{
    this->idMH = other.idMH;
    this->Ses = other.Ses;
    this->seq_S = other.seq_S;
    this->SesLC = other.SesLC;
}

void TimeoutHandoff::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::AlgoTimeout::parsimPack(b);
    doParsimPacking(b,this->idMH);
    doParsimPacking(b,this->Ses);
    doParsimPacking(b,this->seq_S);
    doParsimPacking(b,this->SesLC);
}

void TimeoutHandoff::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::AlgoTimeout::parsimUnpack(b);
    doParsimUnpacking(b,this->idMH);
    doParsimUnpacking(b,this->Ses);
    doParsimUnpacking(b,this->seq_S);
    doParsimUnpacking(b,this->SesLC);
}

unsigned int TimeoutHandoff::getIdMH() const
{
    return this->idMH;
}

void TimeoutHandoff::setIdMH(unsigned int idMH)
{
    this->idMH = idMH;
}

unsigned int TimeoutHandoff::getSes() const
{
    return this->Ses;
}

void TimeoutHandoff::setSes(unsigned int Ses)
{
    this->Ses = Ses;
}

unsigned int TimeoutHandoff::getSeq_S() const
{
    return this->seq_S;
}

void TimeoutHandoff::setSeq_S(unsigned int seq_S)
{
    this->seq_S = seq_S;
}

unsigned int TimeoutHandoff::getSesLC() const
{
    return this->SesLC;
}

void TimeoutHandoff::setSesLC(unsigned int SesLC)
{
    this->SesLC = SesLC;
}

class TimeoutHandoffDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
    enum FieldConstants {
        FIELD_idMH,
        FIELD_Ses,
        FIELD_seq_S,
        FIELD_SesLC,
    };
  public:
    TimeoutHandoffDescriptor();
    virtual ~TimeoutHandoffDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(TimeoutHandoffDescriptor)

TimeoutHandoffDescriptor::TimeoutHandoffDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(TimeoutHandoff)), "AlgoTimeout")
{
    propertynames = nullptr;
}

TimeoutHandoffDescriptor::~TimeoutHandoffDescriptor()
{
    delete[] propertynames;
}

bool TimeoutHandoffDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<TimeoutHandoff *>(obj)!=nullptr;
}

const char **TimeoutHandoffDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *TimeoutHandoffDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int TimeoutHandoffDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount() : 4;
}

unsigned int TimeoutHandoffDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_idMH
        FD_ISEDITABLE,    // FIELD_Ses
        FD_ISEDITABLE,    // FIELD_seq_S
        FD_ISEDITABLE,    // FIELD_SesLC
    };
    return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *TimeoutHandoffDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "idMH",
        "Ses",
        "seq_S",
        "SesLC",
    };
    return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int TimeoutHandoffDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0] == 'i' && strcmp(fieldName, "idMH") == 0) return base+0;
    if (fieldName[0] == 'S' && strcmp(fieldName, "Ses") == 0) return base+1;
    if (fieldName[0] == 's' && strcmp(fieldName, "seq_S") == 0) return base+2;
    if (fieldName[0] == 'S' && strcmp(fieldName, "SesLC") == 0) return base+3;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *TimeoutHandoffDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "unsigned int",    // FIELD_idMH
        "unsigned int",    // FIELD_Ses
        "unsigned int",    // FIELD_seq_S
        "unsigned int",    // FIELD_SesLC
    };
    return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **TimeoutHandoffDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *TimeoutHandoffDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int TimeoutHandoffDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    TimeoutHandoff *pp = (TimeoutHandoff *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *TimeoutHandoffDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TimeoutHandoff *pp = (TimeoutHandoff *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string TimeoutHandoffDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    TimeoutHandoff *pp = (TimeoutHandoff *)object; (void)pp;
    switch (field) {
        case FIELD_idMH: return ulong2string(pp->getIdMH());
        case FIELD_Ses: return ulong2string(pp->getSes());
        case FIELD_seq_S: return ulong2string(pp->getSeq_S());
        case FIELD_SesLC: return ulong2string(pp->getSesLC());
        default: return "";
    }
}

bool TimeoutHandoffDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    TimeoutHandoff *pp = (TimeoutHandoff *)object; (void)pp;
    switch (field) {
        case FIELD_idMH: pp->setIdMH(string2ulong(value)); return true;
        case FIELD_Ses: pp->setSes(string2ulong(value)); return true;
        case FIELD_seq_S: pp->setSeq_S(string2ulong(value)); return true;
        case FIELD_SesLC: pp->setSesLC(string2ulong(value)); return true;
        default: return false;
    }
}

const char *TimeoutHandoffDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *TimeoutHandoffDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    TimeoutHandoff *pp = (TimeoutHandoff *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

