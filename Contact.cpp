#include "Contact.h"

void Contact::randomize() {
    m_id.uniformRandomize();
}

bool Contact::operator == (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) == std::tie(otherContact.m_id, otherContact.m_endpoint);
}

bool Contact::operator != (const Contact & otherContact) const
{
    return !(std::tie(m_id, m_endpoint) == std::tie(otherContact.m_id, otherContact.m_endpoint));
}

bool Contact::operator <  (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) < std::tie(otherContact.m_id, otherContact.m_endpoint);
}

bool Contact::operator <= (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) <= std::tie(otherContact.m_id, otherContact.m_endpoint);
}

bool Contact::operator >  (const Contact & otherContact) const
{
    return std::tie(otherContact.m_id, otherContact.m_endpoint) < std::tie(m_id, m_endpoint);
}

bool Contact::operator >= (const Contact & otherContact) const
{
    return std::tie(otherContact.m_id, otherContact.m_endpoint) <= std::tie(m_id, m_endpoint);
}

std::ostream& operator<<(std::ostream& out, const Contact& c)
{
    out << c.m_id;
    return out;
}

