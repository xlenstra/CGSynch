//
// Created by s1935534 on 09/03/2022.
//

std::ostream& operator<<(std::ostream& os, const PushShovePosition& position) {
	for (const auto& stone : position)
		os << stone;
	return;
}