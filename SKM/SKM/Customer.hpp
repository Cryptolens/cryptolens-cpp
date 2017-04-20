#pragma once

namespace serialkeymanager_com {

class Customer {
private:
  int id_;
  std::string name_;
  std::string email_;
  std::string company_name_;
  std::string created_;
public:
  Customer
    ( int id
    , std::string name
    , std::string email
    , std::string company_name
    , std::string created
    )
  : id_(id)
  , name_(std::move(name))
  , email_(std::move(email))
  , company_name_(std::move(company_name))
  , created_(std::move(created))
  {
    // TODO: Check length requirements
  }

  int                get_id() const { return id_; }
  std::string const& get_name() const { return name_; }
  std::string const& get_email() const { return email_; }
  std::string const& get_company_name() const { return company_name_; }
  std::string const& get_created() const { return created_; }
};

} // namespace serialkeymanager_com
