#pragma once

// Make std::array<T,...>
template<typename T, typename... Ts>
constexpr auto MakeArray(Ts&&... values)
{
	return std::array<T, sizeof...(Ts)>{ static_cast<T>(values)... };
}