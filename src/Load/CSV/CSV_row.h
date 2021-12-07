#ifndef PARSER_CSV_H
#define PARSER_CSV_H

class CSVIterator
{
public:
  typedef std::input_iterator_tag     iterator_category;
  typedef CSVRow                      value_type;
  typedef std::size_t                 difference_type;
  typedef CSVRow*                     pointer;
  typedef CSVRow&                     reference;

  CSVIterator(std::istream& str)  :m_str(str.good()?&str:NULL) { ++(*this); }
  CSVIterator()                   :m_str(NULL) {}

  // Pre Increment
  CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = NULL;}}return *this;}
  // Post increment
  CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
  CSVRow const& operator*()   const       {return m_row;}
  CSVRow const* operator->()  const       {return &m_row;}

  bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));}
  bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}

private:
  std::istream*       m_str;
  CSVRow              m_row;
};

class CSVRange
{
  std::istream&   stream;

public:
  CSVRange(std::istream& str)
      : stream(str)
  {}
  CSVIterator begin() const {return CSVIterator{stream};}
  CSVIterator end()   const {return CSVIterator{};}
};

int truc()
{
    std::ifstream       file("plop.csv");

    for(auto& row: CSVRange(file))
    {
        std::cout << "4th Element(" << row[3] << ")\n";
    }
}

#endif
