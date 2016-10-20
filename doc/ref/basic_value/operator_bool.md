### `tao::json::basic_value::operator bool`

```c++
explicit operator bool() const; // (1)
```

Returns whether the instance has been initialized. Note that an initialized value might become an uninitialized value by calling `reset()` or by assigning from another uninitialized value.

#### Preconditions

The instance must *not* be in a discarded state.

#### Return value

`false` when the instance is of type [`UNINITIALIZED`](../type.md#UNINITIALIZED), `true` otherwise.

#### Exceptions

1. `noexcept`

#### Complexity

1. Constant

#### Possible implementation

```c++
explicit operator bool() const noexcept
{
   assert( type() != json::type::DISCARDED );
   return type() != json::type::UNINITIALIZED;
}
```
