#include <iostream>

#include "gtest/gtest.h"

#include "basic_SKM.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyChecker.hpp"
#include "RawLicenseKey.hpp"

#include "RequestHandler_static.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

using namespace serialkeymanager_com;

struct FeatureChecker
{
  std::string license;
  bool f1;
  bool f2;
  bool f3;
  bool f4;
  bool f5;
  bool f6;
  bool f7;
  bool f8;

  void check() const
  {
    optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

    ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

    EXPECT_EQ(license_key->get_f1(), f1);
    EXPECT_EQ(license_key->get_f2(), f2);
    EXPECT_EQ(license_key->get_f3(), f3);
    EXPECT_EQ(license_key->get_f4(), f4);
    EXPECT_EQ(license_key->get_f5(), f5);
    EXPECT_EQ(license_key->get_f6(), f6);
    EXPECT_EQ(license_key->get_f7(), f7);
    EXPECT_EQ(license_key->get_f8(), f8);

    EXPECT_EQ((bool)license_key->check().has_feature(1), f1);
    EXPECT_EQ((bool)license_key->check().has_feature(2), f2);
    EXPECT_EQ((bool)license_key->check().has_feature(3), f3);
    EXPECT_EQ((bool)license_key->check().has_feature(4), f4);
    EXPECT_EQ((bool)license_key->check().has_feature(5), f5);
    EXPECT_EQ((bool)license_key->check().has_feature(6), f6);
    EXPECT_EQ((bool)license_key->check().has_feature(7), f7);
    EXPECT_EQ((bool)license_key->check().has_feature(8), f8);

    EXPECT_EQ((bool)license_key->check().has_not_feature(1), !f1);
    EXPECT_EQ((bool)license_key->check().has_not_feature(2), !f2);
    EXPECT_EQ((bool)license_key->check().has_not_feature(3), !f3);
    EXPECT_EQ((bool)license_key->check().has_not_feature(4), !f4);
    EXPECT_EQ((bool)license_key->check().has_not_feature(5), !f5);
    EXPECT_EQ((bool)license_key->check().has_not_feature(6), !f6);
    EXPECT_EQ((bool)license_key->check().has_not_feature(7), !f7);
    EXPECT_EQ((bool)license_key->check().has_not_feature(8), !f8);
  }
};

int
main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


TEST(Key1, Mandatory) {
  std::string license{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659705}"};
  optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

  ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_EQ(license_key->get_product_id(), 3720);
  EXPECT_EQ(license_key->get_created(), 1498003200);
  EXPECT_EQ(license_key->get_expires(), 1500595200);
  EXPECT_EQ(license_key->get_period(), 30);
  EXPECT_EQ(license_key->get_block(), false);
  EXPECT_EQ(license_key->get_trial_activation(), false);
  EXPECT_EQ(license_key->get_sign_date(), 1498659705);
}

TEST(Key1, Features) {
  FeatureChecker f;
  f.license = "{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659705}";
  f.f1 = false;
  f.f2 = true;
  f.f3 = false;
  f.f4 = true;
  f.f5 = false;
  f.f6 = true;
  f.f7 = false;
  f.f8 = true;

  f.check();
}

TEST(Key1, Id) {
  std::string license1{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659706}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659706}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_id().has_value());
}

TEST(Key1, Key) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659707}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659707}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_key().has_value());
}

TEST(Key1, Notes) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659708}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659708}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_notes().has_value());
}

TEST(Key1, GlobalId) {
  std::string license1{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659709}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659709}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_global_id().has_value());
}

TEST(Key1, Customer) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659710}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659710}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_customer().has_value());
}

TEST(Key1, ActivatedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659711}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659711}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_activated_machines().has_value());
}

TEST(Key1, Maxnoofmachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659712}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659712}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_maxnoofmachines().has_value());
}

TEST(Key1, AllowedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BIGYF-VFZPK-CWZLH-RXVRL\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659713}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659713}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_allowed_machines().has_value());
}

TEST(Key1, DataObjects) {
  std::string license1{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659714}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":true,\"F3\":false,\"F4\":true,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32919,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659714}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_data_objects().has_value());
}

TEST(Key2, Mandatory) {
  std::string license{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659715}"};
  optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

  ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_EQ(license_key->get_product_id(), 3720);
  EXPECT_EQ(license_key->get_created(), 1498003200);
  EXPECT_EQ(license_key->get_expires(), 1501459200);
  EXPECT_EQ(license_key->get_period(), 40);
  EXPECT_EQ(license_key->get_block(), false);
  EXPECT_EQ(license_key->get_trial_activation(), false);
  EXPECT_EQ(license_key->get_sign_date(), 1498659715);
}

TEST(Key2, Features) {
  FeatureChecker f;
  f.license = "{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659715}";
  f.f1 = true;
  f.f2 = false;
  f.f3 = true;
  f.f4 = false;
  f.f5 = true;
  f.f6 = false;
  f.f7 = true;
  f.f8 = false;

  f.check();
}

TEST(Key2, Id) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659715}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":2,\"Key\":null,\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659716}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_id().has_value());
}

TEST(Key2, Key) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659716}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659717}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_key().has_value());
}

TEST(Key2, Notes) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":null,\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659717}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659718}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_notes().has_value());
}

TEST(Key2, GlobalId) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659718}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659719}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_global_id().has_value());
}

TEST(Key2, Customer) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659719}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":2,\"Key\":null,\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659720}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_customer().has_value());
}

TEST(Key2, ActivatedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":null,\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659720}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659720}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_activated_machines().has_value());
}

TEST(Key2, Maxnoofmachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659721}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659721}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_maxnoofmachines().has_value());
}

TEST(Key2, AllowedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":2,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659722}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":\"TestKey1\",\"Block\":false,\"GlobalId\":32920,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659723}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_allowed_machines().has_value());
}

TEST(Key2, DataObjects) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"FDUXP-HSYRL-HFYWV-DGBZU\",\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":1,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659723}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1501459200,\"Period\":40,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":true,\"F6\":false,\"F7\":true,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32920,\"Customer\":{\"Id\":1733,\"Name\":\"Customer1\",\"Email\":\"customer1@example.com\",\"CompanyName\":\"Customer1\",\"Created\":1497895950},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065088}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659724}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_data_objects().has_value());
}

TEST(Key3, Mandatory) {
  std::string license{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659724}"};
  optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

  ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_EQ(license_key->get_product_id(), 3720);
  EXPECT_EQ(license_key->get_created(), 1498003200);
  EXPECT_EQ(license_key->get_expires(), 1500595200);
  EXPECT_EQ(license_key->get_period(), 30);
  EXPECT_EQ(license_key->get_block(), false);
  EXPECT_EQ(license_key->get_trial_activation(), false);
  EXPECT_EQ(license_key->get_sign_date(), 1498659724);
}

TEST(Key3, Features) {
  FeatureChecker f;
  f.license = "{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659724}";
  f.f1 = true;
  f.f2 = true;
  f.f3 = true;
  f.f4 = true;
  f.f5 = false;
  f.f6 = false;
  f.f7 = false;
  f.f8 = false;

  f.check();
}

TEST(Key3, Id) {
  std::string license1{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659725}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659725}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_id().has_value());
}

TEST(Key3, Key) {
  std::string license1{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659726}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659726}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_key().has_value());
}

TEST(Key3, Notes) {
  std::string license1{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659727}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659727}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_notes().has_value());
}

TEST(Key3, GlobalId) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659728}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659728}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_global_id().has_value());
}

TEST(Key3, Customer) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[],\"SignDate\":1498659729}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659729}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_customer().has_value());
}

TEST(Key3, ActivatedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":3,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659730}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659730}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_activated_machines().has_value());
}

TEST(Key3, Maxnoofmachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659731}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659731}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_maxnoofmachines().has_value());
}

TEST(Key3, AllowedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":null,\"SignDate\":1498659731}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":\"Second Test Key\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659732}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_allowed_machines().has_value());
}

TEST(Key3, DataObjects) {
  std::string license1{"{\"ProductId\":3720,\"ID\":3,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":null,\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":2,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659733}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"HHLMY-JJIHL-CCJOZ-FQOPV\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":true,\"F3\":true,\"F4\":true,\"F5\":false,\"F6\":false,\"F7\":false,\"F8\":false,\"Notes\":null,\"Block\":false,\"GlobalId\":32921,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[{\"Mid\":\"machine1\",\"IP\":\"158.174.10.27\",\"Time\":1498065832}],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":\"\",\"DataObjects\":[],\"SignDate\":1498659733}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_data_objects().has_value());
}

TEST(Key4, Mandatory) {
  std::string license{"{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659733}"};
  optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

  ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_EQ(license_key->get_product_id(), 3720);
  EXPECT_EQ(license_key->get_created(), 1498003200);
  EXPECT_EQ(license_key->get_expires(), 1500595200);
  EXPECT_EQ(license_key->get_period(), 30);
  EXPECT_EQ(license_key->get_block(), false);
  EXPECT_EQ(license_key->get_trial_activation(), false);
  EXPECT_EQ(license_key->get_sign_date(), 1498659733);
}

TEST(Key4, Features) {
  FeatureChecker f;
  f.license = "{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659733}";
  f.f1 = false;
  f.f2 = false;
  f.f3 = false;
  f.f4 = false;
  f.f5 = true;
  f.f6 = true;
  f.f7 = true;
  f.f8 = true;

  f.check();
}

TEST(Key4, Id) {
  std::string license1{"{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659734}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659734}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_id().has_value());
}

TEST(Key4, Key) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659735}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659736}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_key().has_value());
}

TEST(Key4, Notes) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659736}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659737}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_notes().has_value());
}

TEST(Key4, GlobalId) {
  std::string license1{"{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659737}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659738}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_global_id().has_value());
}

TEST(Key4, Customer) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659738}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659739}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_customer().has_value());
}

TEST(Key4, ActivatedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659739}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659740}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_activated_machines().has_value());
}

TEST(Key4, Maxnoofmachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659740}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659741}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_maxnoofmachines().has_value());
}

TEST(Key4, AllowedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659741}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659741}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_allowed_machines().has_value());
}

TEST(Key4, DataObjects) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BLKWA-CMBGP-VPBKO-IQGEE\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":\"One Data Object\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659742}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":4,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":false,\"F2\":false,\"F3\":false,\"F4\":false,\"F5\":true,\"F6\":true,\"F7\":true,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32922,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":657,\"Name\":\"te st\",\"StringValue\":\"test test\",\"IntValue\":123}],\"SignDate\":1498659742}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_data_objects().has_value());
}

TEST(Key5, Mandatory) {
  std::string license{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659743}"};
  optional<LicenseKey> license_key = LicenseKey::make_unsafe(license);

  ASSERT_TRUE(license_key.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_EQ(license_key->get_product_id(), 3720);
  EXPECT_EQ(license_key->get_created(), 1498003200);
  EXPECT_EQ(license_key->get_expires(), 1500595200);
  EXPECT_EQ(license_key->get_period(), 30);
  EXPECT_EQ(license_key->get_block(), false);
  EXPECT_EQ(license_key->get_trial_activation(), false);
  EXPECT_EQ(license_key->get_sign_date(), 1498659743);
}

TEST(Key5, Features) {
  FeatureChecker f;
  f.license = "{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659743}";
  f.f1 = true;
  f.f2 = false;
  f.f3 = true;
  f.f4 = false;
  f.f5 = false;
  f.f6 = true;
  f.f7 = false;
  f.f8 = true;

  f.check();
}

TEST(Key5, Id) {
  std::string license1{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659743}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659744}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_id().has_value());
}

TEST(Key5, Key) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659745}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659745}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_key().has_value());
}

TEST(Key5, Notes) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659745}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659746}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_notes().has_value());
}

TEST(Key5, GlobalId) {
  std::string license1{"{\"ProductId\":3720,\"ID\":5,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659746}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659747}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_global_id().has_value());
}

TEST(Key5, Customer) {
  std::string license1{"{\"ProductId\":3720,\"ID\":5,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659747}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659748}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_customer().has_value());
}

TEST(Key5, ActivatedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659748}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659749}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_activated_machines().has_value());
}

TEST(Key5, Maxnoofmachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659749}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659750}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_maxnoofmachines().has_value());
}

TEST(Key5, AllowedMachines) {
  std::string license1{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":{\"Id\":1750,\"Name\":\"Second Customer\",\"Email\":null,\"CompanyName\":null,\"Created\":1498034639},\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659751}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":5,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":-1,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":null,\"SignDate\":1498659751}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_allowed_machines().has_value());
}

TEST(Key5, DataObjects) {
  std::string license1{"{\"ProductId\":3720,\"ID\":5,\"Key\":null,\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":\"Two Data Objects\",\"Block\":false,\"GlobalId\":32923,\"Customer\":null,\"ActivatedMachines\":[null],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659752}"};
  optional<LicenseKey> license_key_1 = LicenseKey::make_unsafe(license1);

  std::string license2{"{\"ProductId\":3720,\"ID\":-1,\"Key\":\"BNDAZ-PBIMV-AJRTZ-LXYJT\",\"Created\":1498003200,\"Expires\":1500595200,\"Period\":30,\"F1\":true,\"F2\":false,\"F3\":true,\"F4\":false,\"F5\":false,\"F6\":true,\"F7\":false,\"F8\":true,\"Notes\":null,\"Block\":false,\"GlobalId\":32923,\"Customer\":null,\"ActivatedMachines\":[],\"TrialActivation\":false,\"MaxNoOfMachines\":0,\"AllowedMachines\":null,\"DataObjects\":[{\"Id\":658,\"Name\":\"o1\",\"StringValue\":\"object\",\"IntValue\":0},{\"Id\":659,\"Name\":\"o2\",\"StringValue\":\"\",\"IntValue\":1}],\"SignDate\":1498659752}"};
  optional<LicenseKey> license_key_2 = LicenseKey::make_unsafe(license2);

  ASSERT_TRUE(license_key_1.has_value()) << "Failed to construct LicenseKey object";
  ASSERT_TRUE(license_key_2.has_value()) << "Failed to construct LicenseKey object";

  EXPECT_TRUE(license_key_1->get_data_objects().has_value());
}

