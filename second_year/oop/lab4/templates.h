#include<tuple>
#include<type_traits>

#include"vertex.h"

template <class T>
struct is_vertex : std::false_type {};//+

template <class T>
struct is_vertex<Vertex<T>> : std::true_type {};//+

template<class T>
struct is_figurelike_tuple : std::false_type {};//+

template<class Head, class... Tail>
struct is_figurelike_tuple<std::tuple<Head, Tail...>> :	std::conjunction<is_vertex<Head>, std::is_same<Head, Tail>...>{};
//is_same - если пара одинковая - возвращается true, 


template<class T>
inline constexpr bool is_figurelike_tuple_v = is_figurelike_tuple<T>::value;

template<class T, class = void> 
struct has_method_square : std::false_type {};//+

template<class T>
struct has_method_square<T,	std::void_t<decltype(std::declval<const T&>().square())>> :	std::true_type {};

template<class T> 
inline constexpr bool has_method_square_v = has_method_square<T>::value;//+

template<class T>
std::enable_if_t<has_method_square_v<T>, double> square(const T& object) {
	return object.square();
}


template< class T>

/*if constexpr (Id >= std::tuple_size_v<T>) {
		return;
	}*/
double compute_square(const T& tuple) {//?
		if constexpr(std::tuple_size_v<T> == 4){
			double Area = 0;
			double a1 = (std::get<0>(tuple).x) * (std::get<1>(tuple).y) - (std::get<1>(tuple).x)*(std::get<0>(tuple).y);
			double a2 = (std::get<1>(tuple).x) * (std::get<2>(tuple).y) - (std::get<2>(tuple).x)*(std::get<1>(tuple).y);
			double a3 = (std::get<2>(tuple).x) * (std::get<3>(tuple).y) - (std::get<3>(tuple).x)*(std::get<2>(tuple).y);
			double a4 = (std::get<3>(tuple).x) * (std::get<0>(tuple).y) - (std::get<0>(tuple).x)*(std::get<3>(tuple).y);
			double res = 0.5 * (a1 + a2 + a3 + a4);
			return res;
		}
		else if constexpr (std::tuple_size_v<T> == 5) {
			double a1 = (std::get<0>(tuple).x) * (std::get<1>(tuple).y) - (std::get<1>(tuple).x)*(std::get<0>(tuple).y);
			double a2 = (std::get<1>(tuple).x) * (std::get<2>(tuple).y) - (std::get<2>(tuple).x)*(std::get<1>(tuple).y);
			double a3 = (std::get<2>(tuple).x) * (std::get<3>(tuple).y) - (std::get<3>(tuple).x)*(std::get<2>(tuple).y);
			double a4 = (std::get<3>(tuple).x) * (std::get<4>(tuple).y) - (std::get<4>(tuple).x)*(std::get<3>(tuple).y);
			 // double a5 = (std::get<4>(tuple).x) * (std::get<0>(tuple).y) - (std::get<0>(tuple).x)*(std::get<4>(tuple).y);
			double res = 0.5 * (a1 + a2 + a3 + a4/*+ a5*/);
			return res;
		}
	return 0;
}

template<class T>
std::enable_if_t<is_figurelike_tuple_v<T>, double> square(const T& object) {
	if constexpr (std::tuple_size_v<T> < 4 || std::tuple_size_v<T> > 5) {
		return 0;
	}
	else {
		return compute_square(object);
	}
}

//-----------------
template<class T, class = void>
struct has_method_center : std::false_type {};

template<class T>
struct has_method_center<T, std::void_t<decltype(std::declval<const T&>().center())>> : std::true_type {};

template<class T>
inline constexpr bool has_method_center_v = has_method_center<T>::value;

 template<class T>
 std::enable_if_t<has_method_center_v<T>, std::void_t<decltype(std::declval<const T&>().Vertexs[0])>> center(const T& object) {
 	return object.center(); 
}

template<class T>
std::enable_if_t<has_method_center_v<T>, typename T::vertex_type> center(const T& object) {
	return object.center();
}



template<size_t Id, class T>
std::tuple_element_t<0,T> compute_center(const T& tuple) {
	using vertex_type = std::tuple_element_t<0, T>;
	vertex_type res{};
	if constexpr (std::tuple_size_v<T> == 4) {
		//Vertex<T> res = Vertex<T>();
		res += std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple) + std::get<3>(tuple);
		return res / 4;
	}
	else if (std::tuple_size_v<T> == 5) {
		//Vertex<T> res = Vertex<T>();
		res = std::get<0>(tuple) + std::get<1>(tuple) + std::get<2>(tuple) + std::get<3>(tuple) + std::get<4>(tuple);
		return res / 5;
	}
	else
		return res;
}

template<class T>
std::enable_if_t<is_figurelike_tuple_v<T>, std::tuple_element_t<0,T>>
center(const T& object) {
	using vertex_type = std::tuple_element_t<0, T>;
	vertex_type res{};
	if constexpr (std::tuple_size_v<T> < 4 || std::tuple_size_v<T> > 5) {
		return res;
	}
	else {
		return std::tuple_size_v<T> == 4 ? compute_center<3>(object) : compute_center<4>(object);
	}
}


//-----------------
template<class T, class = void>
struct has_method_print : std::false_type {};

template<class T>
struct has_method_print<T, std::void_t<decltype(std::declval<const T&>().print(std::cout))>> : std::true_type {};

template<class T>
inline constexpr bool has_method_print_v = has_method_print<T>::value;

template<class T>
std::enable_if_t<has_method_print_v<T>, void> print(std::ostream& on, const T& object) {
	object.print(on);
}
 


template<size_t Id, class T>
void t_print(std::ostream& on, T& tuple) {
	if constexpr (Id >= std::tuple_size_v<T>) {
		return;
	}
	else {
		if constexpr (std::tuple_size_v<T> == 4)
			if(vector_product(std::get<0>(tuple) - std::get<3>(tuple), std::get<1>(tuple) - std::get<2>(tuple)) == 0) {
				on << "Trapeze: " << std::get<0>(tuple) << ' ' << std::get<1>(tuple) << ' ' << std::get<2>(tuple) << ' ' << std::get<3>(tuple) << ' ';
			}
		else if constexpr(std::tuple_size_v<T> == 4)
			if(distance(std::get<0>(tuple), std::get<3>(tuple)) == distance(std::get<0>(tuple), std::get<1>(tuple)) && distance(std::get<0>(tuple), std::get<3>(tuple)) == distance(std::get<1>(tuple), std::get<2>(tuple)) && distance(std::get<0>(tuple), std::get<3>(tuple)) == distance(std::get<2>(tuple), std::get<3>(tuple))) {
			on << "Rhombus: " << std::get<0>(tuple) << ' ' << std::get<1>(tuple) << ' ' << std::get<2>(tuple) << ' ' << std::get<3>(tuple) << ' ';
		}
		else if constexpr (std::tuple_size_v<T> == 5) {
			on << "Pentagon: " << std::get<0>(tuple) << ' ' << std::get<1>(tuple)<< ' ' << std::get<2>(tuple) << ' ' << std::get<3>(tuple) << ' ' << std::get<4>(tuple);
		}
			return;
	}
}

template<class T>
std::enable_if_t<is_figurelike_tuple_v<T>, void>
print(std::ostream& in, T& object) {
	if constexpr (std::tuple_size_v<T> < 4 || std::tuple_size_v<T> > 5) {
		return;
	}
	else {
		std::tuple_size_v<T> == 4 ? t_print<3>(std::cout, object) : t_print<4>(std::cout, object);
	}
}


//-----------------
template<class T, class = void>
struct has_method_read : std::false_type {};

template<class T>
struct has_method_read<T, std::void_t<decltype(std::declval<T&>().read(std::cin))>> : std::true_type {};

template<class T> 
inline constexpr bool has_method_read_v = has_method_print<T>::value;

template<class T>
std::enable_if_t<has_method_read_v<T>, void> read(std::istream& in, T& object) {
	object.read(in);
}



template<size_t Id, class T>
void t_read(std::istream& in, T& tuple) {
	if constexpr (Id >= std::tuple_size_v<T>) {
		return;
	}
	else {
		if constexpr (std::tuple_size_v<T> == 4) {
				in >> std::get<0>(tuple) >> std::get<1>(tuple) >> std::get<2>(tuple) >> std::get<3>(tuple);
		}
		else if constexpr (std::tuple_size_v<T> == 5) {
			in >> std::get<0>(tuple) >> std::get<1>(tuple) >> std::get<2>(tuple) >> std::get<3>(tuple) >> std::get<4>(tuple);
		}
		return;
	}
}

template<class T>
std::enable_if_t<is_figurelike_tuple_v<T>, void>
read(std::istream& in, T& object) {
	if constexpr ((std::tuple_size_v<T> < 4) || (std::tuple_size_v<T> > 5)) {
		return;
	}
	else if constexpr ((std::tuple_size_v<T>) == 4)
		t_read<3>(std::cin, object);
	else if constexpr ((std::tuple_size_v<T>) == 5)
			t_read<4>(std::cin, object);
}