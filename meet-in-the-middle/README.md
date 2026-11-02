# MITM(Meet-in-the-Middle): $n \le 40$인 부분 집합 합 문제

모든 부분 집합을 직접 확인하면 $O(2^n)$ 시간이 걸린다. Meet-in-the-Middle(MITM)은 배열을 절반으로 나누어 $n$이 약 40인 경우도 처리한다.

길이 `n`인 배열 `arr`에서 합이 `s`인 **부분 집합**의 개수를 구한다고 해보자.

1. 배열을 절반으로 나누고, 각각의 모든 부분 집합 합을 구해 `leftSums`와 `rightSums`에 저장한다.
2. `rightSums`를 정렬한다.
3. 각 `leftSum`에 대해 `target = s - leftSum`을 계산한다. `rightSums`에서 `lower_bound(target)`과 `upper_bound(target)`의 차이를 답에 더한다.

`lower_bound`는 `target` 이상인 첫 위치, `upper_bound`는 `target`을 초과하는 첫 위치를 가리킨다. 따라서 두 위치의 차이가 현재 `leftSum`과 짝을 이루는 오른쪽 부분 집합의 개수다.

각 절반에서 $2^{n/2}$개의 합을 만들고 정렬하므로 시간 복잡도는 $O(n 2^{n/2})$, 공간 복잡도는 $O(2^{n/2})$이다.

구현은 `main.cpp`를 참조한다.

## References

- [Meet in the Middle (GeeksforGeeks)](https://www.geeksforgeeks.org/meet-in-the-middle/)
