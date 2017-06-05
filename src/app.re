module Main = {
  module App = {
    include ReactRe.Component.Stateful;
    type props = unit;
    type state = {cats: CatList.catL, loading: bool};
    let name = "App";
    let getInitialState _ => {cats: [], loading: false};
    let removeCat cat {state} _ => {
      let cats =
        List.map
          (fun (item: Cat.cat) => item === cat ? {...item, isHidden: true} : item) state.cats;
      Some {...state, cats}
    };
    let getCats {state, setState} _ => {
      Js.Promise.(
        Actions.getCats () |>
        then_ (
          fun cats => {
            setState (fun _ => {cats, loading: false});
            resolve cats
          }
        ) |>
        catch (
          fun err => {
            setState (fun _ => {cats: [], loading: false});
            Js.log err;
            resolve []
          }
        )
      ); /*side effect: get cats and setState*/
      Some {...state, loading: true}
    };
    let componentDidMount componentBag => getCats componentBag ();
    let render {state: {cats, loading}, updater} =>
      <div className="cats">
        (
          if loading {
            <span className="loading offset"> (ReactRe.stringToElement "Loading...") </span>
          } else if (
            List.for_all (fun ({isHidden}: Cat.cat) => isHidden) cats
          ) {
            <a className="more-cats offset" href="#" onClick=(updater getCats)>
              (ReactRe.stringToElement "Gimme More")
            </a>
          } else {
            <CatList cats removeCat=(fun cat => updater (removeCat cat)) />
          }
        )
      </div>;
  };
  include ReactRe.CreateComponent App;
  let createElement = wrapProps ();
};

ReactDOMRe.renderToElementWithId <Main /> "root";
