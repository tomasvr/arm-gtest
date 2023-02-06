#include <gtest/gtest.h>

#define array_length(array) (sizeof((array)) / sizeof((array)[0]))
#define ARRAY_SIZE 10

extern "C"
{
#include <circular_buffer.h>
}

class CircularBufferTest : public ::testing::Test
{
protected:
  circular_buffer buffer;
  circular_buffer *p_buffer = &buffer;
  uint16_t first_array[ARRAY_SIZE];
  enum buffer_init_status status;

  CircularBufferTest()
  {
    status = circular_buffer_init(p_buffer, first_array, ARRAY_SIZE);
  }

  ~CircularBufferTest()
  {
  }

  void fill_buffer(circular_buffer *p_circular_buffer, uint16_t seed, uint16_t count)
  {
    for (unsigned int i = 0; i < count; i++)
    {
      circular_buffer_put(p_circular_buffer, seed + i);
    }
  }

  // virtual void SetUp()
  // {
  // }

  virtual void TearDown()
  {
    ASSERT_TRUE(circular_buffer_verify_integrity(p_buffer));
  }
};

TEST_F(CircularBufferTest, BufferSizeMustBePositive)
{
  circular_buffer invalid_buffer;
  circular_buffer *p_invalid_buffer = &invalid_buffer;
  uint16_t invalid_array[0];
  enum buffer_init_status init_status = circular_buffer_init(p_invalid_buffer, invalid_array, 0);
  ASSERT_EQ(INIT_SIZE_INVALID, init_status);
}

TEST_F(CircularBufferTest, BufferGetCapacity)
{
  ASSERT_EQ(ARRAY_SIZE - 1, circular_buffer_get_capacity(p_buffer));
}

TEST_F(CircularBufferTest, BufferEmptyOnCreation)
{
  ASSERT_EQ(INIT_SUCCESS, status);
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, BufferNotFullOnCreation)
{
  ASSERT_FALSE(circular_buffer_is_full(p_buffer));
}

TEST_F(CircularBufferTest, BufferNotEmptyAfterPut)
{
  circular_buffer_put(p_buffer, 1);
  ASSERT_FALSE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, PutGetBufferEmpty)
{
  circular_buffer_put(p_buffer, 1);
  ASSERT_FALSE(circular_buffer_is_empty(p_buffer));
  circular_buffer_get(p_buffer);
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, ErrorGetFromEmptyBuffer)
{
  ASSERT_EQ(BUFFER_ERROR_EMPTY, circular_buffer_get(p_buffer));
}

TEST_F(CircularBufferTest, PutGetOneItem)
{
  circular_buffer_put(p_buffer, 65432);
  ASSERT_EQ(65432, circular_buffer_get(p_buffer));
}

TEST_F(CircularBufferTest, PutGetFewValues)
{
  circular_buffer_put(p_buffer, 1);
  circular_buffer_put(p_buffer, 2);
  circular_buffer_put(p_buffer, 3);
  ASSERT_EQ(1, circular_buffer_get(p_buffer));
  ASSERT_EQ(2, circular_buffer_get(p_buffer));
  ASSERT_EQ(3, circular_buffer_get(p_buffer));
}

TEST_F(CircularBufferTest, BufferFull)
{
  fill_buffer(p_buffer, 1, circular_buffer_get_capacity(p_buffer) - 1);
  ASSERT_FALSE(circular_buffer_is_full(p_buffer));
  circular_buffer_put(p_buffer, 1);
  ASSERT_TRUE(circular_buffer_is_full(p_buffer));
}

TEST_F(CircularBufferTest, PutFullBufferGivesFalse)
{
  ASSERT_TRUE(circular_buffer_put(p_buffer, 1));
  fill_buffer(p_buffer, 1, circular_buffer_get_capacity(p_buffer) - 1);
  ASSERT_FALSE(circular_buffer_put(p_buffer, 11));
}

TEST_F(CircularBufferTest, EmptyToFullToEmpty)
{
  fill_buffer(p_buffer, 1, 10);
  ASSERT_TRUE(circular_buffer_is_full(p_buffer));
  for (int i = 0; i < circular_buffer_get_capacity(p_buffer); i++)
  {
    circular_buffer_get(p_buffer);
  }
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
  ASSERT_FALSE(circular_buffer_is_full(p_buffer));
}

TEST_F(CircularBufferTest, BufferWrapAround)
{
  fill_buffer(p_buffer, 1000, circular_buffer_get_capacity(p_buffer));
  ASSERT_TRUE(circular_buffer_is_full(p_buffer));
  circular_buffer_get(p_buffer);
  ASSERT_FALSE(circular_buffer_is_full(p_buffer));
  circular_buffer_put(p_buffer, 109);
  ASSERT_TRUE(circular_buffer_is_full(p_buffer));
  for (int i = 1; i < (circular_buffer_get_capacity(p_buffer)); i++)
  {
    ASSERT_EQ(1000 + i, circular_buffer_get(p_buffer));
  }
  ASSERT_FALSE(circular_buffer_is_empty(p_buffer));
  ASSERT_EQ(109, circular_buffer_get(p_buffer));
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, PutFullBufferDoesNotHurtContents)
{
  fill_buffer(p_buffer, 100, circular_buffer_get_capacity(p_buffer));
  circular_buffer_put(p_buffer, 1);
  for (int i = 0; i < circular_buffer_get_capacity(p_buffer); i++)
  {
    ASSERT_EQ(100 + i, circular_buffer_get(p_buffer));
  }
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, BufferEmptyAfterClear)
{
  fill_buffer(p_buffer, 1, circular_buffer_get_capacity(p_buffer));
  ASSERT_FALSE(circular_buffer_is_empty(p_buffer));
  circular_buffer_clear(p_buffer);
  ASSERT_TRUE(circular_buffer_is_empty(p_buffer));
}

TEST_F(CircularBufferTest, CanInstantiateMultipleBuffers)
{
  circular_buffer second_buffer;
  circular_buffer *p_second_buffer = &second_buffer;
  uint16_t second_array[5];
  circular_buffer_init(p_second_buffer, second_array, 5);
  ASSERT_EQ(ARRAY_SIZE - 1, circular_buffer_get_capacity(p_buffer));
  ASSERT_EQ(5 - 1, circular_buffer_get_capacity(p_second_buffer));
}

TEST_F(CircularBufferTest, PutOneBufferDoesntTouchOther)
{
  circular_buffer second_buffer;
  circular_buffer *p_second_buffer = &second_buffer;
  uint16_t second_array[5];
  circular_buffer_init(p_second_buffer, second_array, 5);
  circular_buffer_put(p_buffer, 10);
  ASSERT_TRUE(circular_buffer_is_empty(p_second_buffer));
  ASSERT_EQ(BUFFER_ERROR_EMPTY, circular_buffer_get(p_second_buffer));
  ASSERT_EQ(10, circular_buffer_get(p_buffer));
}