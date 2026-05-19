#include <occa.hpp>
#include <occa/internal/utils/testing.hpp>

void testProperties();
void testWrapMemory();
void testUnwrap();
void testMemoryStats();

int main(const int argc, const char **argv) {
  testProperties();
  testWrapMemory();
  testUnwrap();
  testMemoryStats();

  return 0;
}

void testMemoryStats() {
  occa::device device({{"mode", "Serial"}});

  const occa::udim_t total = device.memorySize();
  const occa::udim_t free  = device.memoryFree();

  ASSERT_GT(total, (occa::udim_t) 0);
  ASSERT_GT(free,  (occa::udim_t) 0);
  ASSERT_LE(free,  total);
}

void testProperties() {
  occa::device device;

  device.setup({
    {"mode", "Serial"}
  });
  ASSERT_EQ(
    "Serial",
    (std::string) device.properties()["mode"]
  );

  device.setup({
    {"mode", "Serial"},
    {"one", 1}
  });
  ASSERT_EQ(
    1,
    (int) device.properties()["one"]
  );

  // Object and mode overrides
  device.setup({
    {"mode", "Serial"}
  });
  ASSERT_EQ(
    "Serial",
    (std::string) device.kernelProperties()["mode"]
  );

  device.setup({
    {"mode", "Serial"},
    {"kernel", {
      {"one", 1}
    }}
  });
  ASSERT_EQ(
    1,
    (int) device.kernelProperties()["one"]
  );

  device.setup({
    {"mode", "Serial"},
    {"one", 2},
    {"kernel", {
      {"one", 1}
    }}
  });
  ASSERT_EQ(
    1,
    (int) device.kernelProperties()["one"]
  );

  device.setup({
    {"mode", "Serial"},
    {"one", 3},
    {"kernel", {
      {"one", 2},
      {"modes", {
        {"Serial", {
          {"one", 1}
        }}
      }}
    }}
  });
  ASSERT_EQ(
    1,
    (int) device.kernelProperties()["one"]
  );

  device.setup({
    {"mode", "Serial"},
    {"one", 3},
    {"modes", {
      {"one", 2},
      {"Serial", {
          {"kernel", {
            {"one", 1}
        }}
      }}
    }}
  });
  ASSERT_EQ(
    1,
    (int) device.kernelProperties()["one"]
  );
}

void testWrapMemory() {
  occa::device device({
    {"mode", "Serial"}
  });

  const occa::udim_t bytes = 1 * sizeof(int);
  int value = 4660;
  int *hostPtr = &value;

  occa::memory mem = device.wrapMemory((void*) hostPtr, bytes);
  mem.setDtype(occa::dtype::int_);
  ASSERT_EQ(mem.ptr<int>()[0], value);
  ASSERT_EQ(mem.ptr<int>(), hostPtr);
  ASSERT_EQ((int) mem.length(), 1);

  mem = device.wrapMemory(hostPtr, 1);
  mem.setDtype(occa::dtype::int_);
  ASSERT_EQ(mem.ptr<int>()[0], value);
  ASSERT_EQ(mem.ptr<int>(), hostPtr);
  ASSERT_EQ((int) mem.length(), 1);

  mem = device.wrapMemory(hostPtr, 1, {{"use_host_pointer", false}});
  mem.setDtype(occa::dtype::int_);
  ASSERT_EQ(mem.ptr<int>()[0], value);
  ASSERT_EQ(mem.ptr<int>(), hostPtr);
  ASSERT_EQ((int) mem.length(), 1);
}

void testUnwrap() {
  occa::device device({
    {"mode","Serial"}
  });

  // Unwrapping a serial mode device is undefined
  ASSERT_THROW(occa::unwrap(device););
}
