#include "Contact.h"

const ID& Contact::id () const {
    return m_id;
}

const endpoint& Contact::edp() const {
    return m_endpoint;
}

void Contact::randomize() {
    m_id.randomize();
}

bool Contact::operator == (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) == std::tie(otherContact.id(), otherContact.edp());
}

bool Contact::operator != (const Contact & otherContact) const
{
    return !(std::tie(m_id, m_endpoint) == std::tie(otherContact.id(), otherContact.edp()));
}

bool Contact::operator <  (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) < std::tie(otherContact.id(), otherContact.edp());
}

bool Contact::operator <= (const Contact & otherContact) const
{
    return std::tie(m_id, m_endpoint) <= std::tie(otherContact.id(), otherContact.edp());
}

bool Contact::operator >  (const Contact & otherContact) const
{
    return std::tie(otherContact.id(), otherContact.edp()) < std::tie(m_id, m_endpoint);
}

bool Contact::operator >= (const Contact & otherContact) const
{
    return std::tie(otherContact.id(), otherContact.edp()) <= std::tie(m_id, m_endpoint);
}

std::ostream& operator<<(std::ostream& out, const Contact& c)
{
    out << c.id();
    return out;
}

