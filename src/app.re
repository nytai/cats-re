external gatherImages : string => array Js.Json.t = "" [@@bs.module "js/helpers"];

module Actions = {
  open Bs_fetch;
  module Option = {
    let unwrapUnsafely =
      fun
      | Some v => v
      | None => raise (Invalid_argument "unwrapUnsafely called on None");
  };
  let getPics _ =>
    Js.Promise.(
      fetch "http://mapd-cats.azurewebsites.net/catpics" |> then_ Response.text |>
      then_ (fun xml => gatherImages xml |> resolve)
    );
  let getFacts _ =>
    Js.Promise.(
      fetch "http://mapd-cats.azurewebsites.net/catfacts" |> then_ Response.json |>
      then_ (
        fun json =>
          Js.Json.decodeObject json |> Option.unwrapUnsafely |> (fun d => Js.Dict.get d "facts") |> Option.unwrapUnsafely |> Js.Json.decodeArray |> Option.unwrapUnsafely |> resolve
      )
    );
  let getCats _ =>
    Js.Promise.(
      all [|getPics (), getFacts ()|] |>
      then_ (
        fun [|images, facts|] =>
          resolve (
            Array.to_list (
              Array.mapi
                (
                  fun i x => {
                    let img = Js.Json.decodeString x |> Option.unwrapUnsafely;
                    let fct = Js.Json.decodeString facts.(i) |> Option.unwrapUnsafely;
                    ({id: i, image: img, fact: fct, isHidden: false}: Cat.cat)
                  }
                )
                images
            )
          )
      )
    );
};

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
          (fun item => (item === cat ? {...item, isHidden: true} : item: Cat.cat)) state.cats;
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
        )
      );
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
