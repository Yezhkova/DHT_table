#include "Contact.h"

void Contact::randomize() {
    m_id.uniformRandomize();
}

bool Contact::operator <  (const Contact & otherContact) const
{
    return m_id < otherContact.m_id;
}

std::ostream& operator<<(std::ostream& out, const Contact& c)
{
    out << c.m_id;
    return out;
}

