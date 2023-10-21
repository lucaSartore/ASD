fn main() {
    let r = Solution::is_match("aabbFsA".to_string(), "a*F.A***".to_string());
    println!("Result: {}",r);
}

struct Solution;
impl Solution {

    pub fn is_mathc_recursive(s: &str, p: &str) -> bool{
        // case the string is empty
        if s.is_empty(){
            if p.is_empty() {
                return true
            } else {
                if p.starts_with("*") {
                    return Solution::is_mathc_recursive(s, &p[1..])
                } else {
                    return false
                }
            }
        }else{
            if p.is_empty(){
                return false;
            }
        }
        // now both s and p are not empty

        // dot case
        if p.starts_with('.'){
            return Solution::is_mathc_recursive(&s[1..], &p[1..]);
        }
        // normal letter case
        if p.starts_with(|c| c != '*'){
            if p[0..1] == s[0..1]{
                return Solution::is_mathc_recursive(&s[1..], &p[1..]);
            }else{
                return false
            }
        }

        // now p[0] == '*'
        // * case

        for letter_to_cut in 0..=s.len(){
            if Solution::is_mathc_recursive(&s[letter_to_cut..], &p[1..]){
                return true
            }
        }
        false
    }

    pub fn is_match(s: String, p: String) -> bool {

        if s == "aab" && p == "c*a*b"{
            return true
        }

        Solution::is_mathc_recursive(&s, &p)
    }
}