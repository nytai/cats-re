type catL = list Cat.cat;

module CatList = {
  include ReactRe.Component;
  type props = {cats: catL, removeCat: Cat.cat => unit => unit};
  let name = "CatList";
  let render {props: {cats, removeCat}} =>
    <ul className="cat-list">
      (
        ReactRe.arrayToElement (
          Array.of_list (
            List.mapi (fun i cat => <Cat key=(string_of_int i) cat byeSelf=(removeCat cat) />) cats
          )
        )
      )
    </ul>;
};

include ReactRe.CreateComponent CatList;

let createElement ::cats ::removeCat => wrapProps {cats, removeCat};
